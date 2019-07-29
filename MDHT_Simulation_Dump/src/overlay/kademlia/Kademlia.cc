//
// Copyright (C) 2006 Institut fuer Telematik, Universitaet Karlsruhe (TH)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

/**
 * @file Kademlia.cc
 * @author Sebastian Mies, Ingmar Baumgart, Bernhard Heep
 */

#include "Kademlia.h"

#include <assert.h>
#include <algorithm>

#include <IPAddressResolver.h>
#include <IPvXAddress.h>
#include <IInterfaceTable.h>
#include <IPv4InterfaceData.h>
#include "TopologyVis.h"
#include <AbstractLookup.h>
#include <LookupListener.h>
#include <RpcMacros.h>
#include <BootstrapList.h>

#if 0
#define BUCKET_CONSISTENCY(msg) \
    do {\
        bool stFull = false;\
        int z = 0;\
        if (siblingTable != NULL && siblingTable->size() > 0) {\
            stFull = siblingTable->isFull();\
            z = routingBucketIndex(siblingTable->back().getKey()) - 1;\
            if (routingTable[z - 1] != NULL && routingTable[z - 1]->size())\
                breakpoint(#msg);\
        }\
        for (int y = 0; y < (z - 2); ++y) {\
            if ((routingTable[y] != NULL && routingTable[y]->size() > k) ||\
                (routingTable[y] != NULL && routingTable[y]->size() && !stFull))\
                breakpoint(#msg);\
        }\
    } while (false)
#else
#define BUCKET_CONSISTENCY(msg)
#endif

Define_Module(Kademlia);

std::ostream& operator<<(std::ostream& os, const KademliaBucket* n)
{
    if (n == NULL)
        os << "NULL";
    else {
        for (KademliaBucket::const_iterator i=n->begin(); i !=n->end(); i++) {
            os << *i << endl;
        }
        os << "last usage = " << n->getLastUsage()
           << ", last update = " << n->getLastUpdate();
    }
    return os;
};

class KademliaLookupListener : public LookupListener
{
private:
    Kademlia* overlay;
public:
    KademliaLookupListener(Kademlia* overlay)
    {
        this->overlay = overlay;
    }

    virtual void lookupFinished(AbstractLookup *lookup)
    {
        overlay->lookupFinished(lookup->isValid());
        delete this;
    }
};

//-----------------------------------------------------------------------------

void Kademlia::initializeOverlay(int stage)
{
    if (stage != MIN_STAGE_OVERLAY)
        return;

    // Kademlia provides KBR services
    kbr = true;

    // setup kademlia parameters
    minSiblingTableRefreshInterval = par("minSiblingTableRefreshInterval");
    minBucketRefreshInterval = par("minBucketRefreshInterval");
    exhaustiveRefresh = par("exhaustiveRefresh");
    maxStaleCount = par("maxStaleCount");
    pingNewSiblings = par("pingNewSiblings");

    k = par("k");
    b = par("b");
    s = par("s");

    // calculate number of buckets: ( (2^b)-1 ) * ( keylength / b )
    numBuckets = ((1L << b) - 1L) * (OverlayKey::getLength() / b);

    // init routing and sibling table
    siblingTable = new KademliaBucket(s * 5, NULL);

    // initialize pointers
    routingTable.assign(numBuckets, (KademliaBucket*)NULL);

    WATCH_VECTOR(*siblingTable);
    WATCH_VECTOR(routingTable);

    // self-message
    bucketRefreshTimer = new cMessage("bucketRefreshTimer");

    // statistics
    bucketRefreshCount = 0;
    siblingTableRefreshCount = 0;
    nodesReplaced = 0;

    comparator = NULL;
}

Kademlia::Kademlia()
{
    siblingTable = NULL;
    comparator = NULL;
}

Kademlia::~Kademlia()
{
    routingDeinit();

    replacementCache.clear();
    delete siblingTable;
    delete comparator;
    cancelAndDelete(bucketRefreshTimer);
}

void Kademlia::finishOverlay()
{
    simtime_t time = globalStatistics->calcMeasuredLifetime(creationTime);
    if (time < GlobalStatistics::MIN_MEASURED) return;

    globalStatistics->addStdDev("Kademlia: Nodes replaced in buckets/s",
                                nodesReplaced / time);
    globalStatistics->addStdDev("Kademlia: Bucket Refreshes/s",
                                bucketRefreshCount / time);
    globalStatistics->addStdDev("Kademlia: Sibling Table Refreshes/s",
                                siblingTableRefreshCount / time);
}

void Kademlia::joinOverlay()
{
    // remove current node handle from the bootstrap list
    if (!thisNode.getKey().isUnspecified()) {
        bootstrapList->removeBootstrapNode(thisNode);
    }

    // initialize routing
    routingDeinit();
    routingInit();

    TransportAddress handle = bootstrapList->getBootstrapNode();

    if (!handle.isUnspecified()) {
        // ping the bootstrap node to start bootstrapping
        pingNode(handle);

        /*
        FindNodeCall* findNodeCall = new FindNodeCall();
        findNodeCall->setLookupKey(handle.getKey());
        findNodeCall->setSrcNode(getThisNode());
        findNodeCall->setNumRedundantNodes(getMaxNumRedundantNodes());
        findNodeCall->setNumSiblings(getMaxNumSiblings());
        findNodeCall->setBitLength(FINDNODECALL_L(call));

        RECORD_STATS(numFindNodeSent++;
            bytesFindNodeSent += findNodeCall->getByteLength());

        sendUdpRpcCall(handle, findNodeCall);
         */

    } else {
        // we're the only node in the network
        state = READY;
        setOverlayReady(true);

        // schedule bucket refresh timer
        cancelEvent(bucketRefreshTimer);
        scheduleAt(simTime(), bucketRefreshTimer);
    }
}

//-----------------------------------------------------------------------------

void Kademlia::routingInit()
{
    // set join state
    state = INIT;

    setOverlayReady(false);

    // setup comparator
    comparator = new KeyDistanceComparator<KeyXorMetric>( thisNode.getKey() );

    siblingTable->setComparator(comparator);

    updateTooltip();
    BUCKET_CONSISTENCY(routingInit: end);
}

void Kademlia::routingDeinit()
{
    // delete buckets
    for (uint32_t i = 0; i < routingTable.size(); i++) {
        if (routingTable[i] != NULL) {
            delete routingTable[i];
            routingTable[i] = NULL;
        }
    }

    if (siblingTable != NULL) {
        siblingTable->clear();
    }

    if (comparator != NULL) {
        delete comparator;
        comparator = NULL;
    }
}

int Kademlia::getMaxNumSiblings()
{
    return s;
}

int Kademlia::getMaxNumRedundantNodes()
{
    return k;
}

int Kademlia::routingBucketIndex(const OverlayKey& key, bool firstOnLayer)
{
    // calculate XOR distance
    OverlayKey delta = key ^ getThisNode().getKey();

    // find first subinteger that is not zero...
    int i;
    for (i = key.getLength() - b; i >= 0 && delta.getBitRange(i, b) == 0;
         i -= b);

    if (i < 0)
        return -1;

    if (!firstOnLayer)
        return (i / b) * ((1 << b) - 1) + (delta.getBitRange(i, b) - 1);
    else
        return (i / b) * ((1 << b) - 1) + (pow(2, b) - 2);
}

KademliaBucket* Kademlia::routingBucket(const OverlayKey& key, bool ensure)
{
    // get bucket index
    int num = routingBucketIndex(key);
    if (num < 0)
        return NULL;

    // get bucket and allocate if necessary
    KademliaBucket* bucket = routingTable[ num ];
    if (bucket == NULL && ensure)
        bucket = routingTable[ num ] = new KademliaBucket( k, comparator );

    // return bucket
    return bucket;
}

bool Kademlia::routingAdd(const NodeHandle& handle, bool isAlive, simtime_t rtt)
{
    BUCKET_CONSISTENCY(routingAdd: start);
    // never add unspecified node handles
    if (handle.isUnspecified() || handle.getKey() == getThisNode().getKey() )
        return false;

    // bucket index
    KademliaBucket::iterator i;
    bool result = false;

    // convert node handle
    KademliaBucketEntry kadHandle = handle;
    kadHandle.setRtt(rtt);
    kadHandle.setLastSeen(simTime());

    /* check if node is already a sibling -----------------------------------*/
    if ((i = siblingTable->findIterator(handle.getKey()))
         != siblingTable->end()) {
        // not alive? -> do not change routing information
        if (isAlive) {
            if (kadHandle.getRtt() != i->getRtt()) {
                siblingTable->setLastUpdate(simTime());
                if (kadHandle.getRtt() == MAXTIME)
                    kadHandle.setRtt(i->getRtt());
            }
            // refresh sibling
            (*i) = kadHandle;
        }
        BUCKET_CONSISTENCY(routingAdd: node is sibling);
        return true;
    }

    /* check if node is already in a bucket ---------------------------------*/
    KademliaBucket* bucket = routingBucket(handle.getKey(), false);
    if (bucket != NULL && (i = bucket->findIterator(handle.getKey() ) )
            != bucket->end() ) {
        // not alive? -> do not change routing information
        if (isAlive) {
            if (kadHandle.getRtt() == MAXTIME) {
                kadHandle.setRtt(i->getRtt());
            }

            // remove old handle
            bucket->erase(i);
            // re-add to tail
            bucket->push_back(kadHandle);
            bucket->setLastUpdate(simTime());
        }
        BUCKET_CONSISTENCY(routingAdd: node is in bucket);
        return true;
    }

    /* check if node can be added to the sibling list -----------------------*/
    if (siblingTable->isAddable(handle) ) {

        bool finished = false;
        int siblingPos = -1;

        // check if sibling list is full so a handle is preemted from the list
        if (siblingTable->isFull()) {
            // get handle thats about to be preempted
            KademliaBucketEntry oldHandle = siblingTable->back();
            assert(oldHandle.getKey() != kadHandle.getKey());
            // add handle to the sibling list
            siblingPos = siblingTable->add(kadHandle);

            // change, so that the preempted handle is added to a bucket
            kadHandle = oldHandle;

            // return always true, since the handle has been added
            result = true;
        } else {
            // simply add the handle and stop
            siblingPos = siblingTable->add(kadHandle);

            // don't need to add kadHandle also to regular buckets
            finished = true;
        }
        assert(siblingPos > -1);

        // ping new siblings
        if ((pingNewSiblings && !isAlive)) {
            pingNode(handle);
        }

        siblingTable->setLastUpdate(simTime());

        updateTooltip();

        // call update() for real siblings
        if (siblingPos < getMaxNumSiblings()) {
            if (siblingTable->size() > (uint32_t)getMaxNumSiblings()) {
                // removed old sibling
                NodeHandle& removedSibling = siblingTable->at(getMaxNumSiblings());
                deleteOverlayNeighborArrow(removedSibling);
                callUpdate(removedSibling, false);
            }
            // new sibling
            showOverlayNeighborArrow(handle, false,
                                     "m=m,50,100,50,100;ls=green,1");
            callUpdate(handle, true);
        }

        if (finished) {
            BUCKET_CONSISTENCY(routingAdd: node is now sibling);
            return true;
        }
    }

    /* add node to the appropriate bucket, if not full ---------------------*/
    bucket = routingBucket(kadHandle.getKey(), true);
    if (!bucket->isFull()) {
        EV << "[Kademlia::routingAdd()]\n"
           << "    Adding new node " << kadHandle
           << " to bucket " << routingBucketIndex(kadHandle.getKey())
           << endl;

        bucket->push_back(kadHandle);
        bucket->setLastUpdate(simTime());
        result = true;
    } else if (isAlive) {
        //TODO parameter for usage of replacement cache
        // save candidate, ping head,
        KademliaBucket::iterator it = bucket->begin();
        while (it != bucket->end() &&
               replacementCache.find(*it) != replacementCache.end()) {
            ++it;
        }
        if (it != bucket->end()) {
            replacementCache.insert(std::make_pair(*it, kadHandle));
            //TODO paper: probing should be delayed until useful messages are
            //there to send to head
            pingNode(*it);
        }
    }

    BUCKET_CONSISTENCY(routingAdd: end);
    return result;
}

bool Kademlia::routingRemove(const OverlayKey& key)
{
    return routingTimeout(key, true);
}

bool Kademlia::routingTimeout(const OverlayKey& key, bool immediately)
{
    BUCKET_CONSISTENCY(routingTimeout: start);
    // key unspecified? yes -> ignore
    if (key.isUnspecified())
        return false;

    // bucket index
    KademliaBucket::iterator i;

    /* check if the node is one of the siblings -----------------------------*/
    if ((i = siblingTable->findIterator(key)) != siblingTable->end()) {

        i->incStaleCount();

        if (i->getStaleCount() > maxStaleCount || immediately) {
            // remove from sibling table
            NodeHandle oldSibling = *i;
            siblingTable->erase(i);

            if (siblingTable->size() < (uint32_t)getMaxNumSiblings()) {
                // no new replacement sibling
                deleteOverlayNeighborArrow(oldSibling);
                callUpdate(oldSibling, false);
            } else if (comparator->compare(oldSibling.getKey(),
                               siblingTable->at(getMaxNumSiblings() - 1).getKey()) < 0) {
                // failed sibling was replaced by next closest node in siblingTable
                deleteOverlayNeighborArrow(oldSibling);
                callUpdate(oldSibling, false);

                showOverlayNeighborArrow(siblingTable->at(getMaxNumSiblings() - 1),
                                         false, "m=m,50,100,50,100;ls=green,1");
                callUpdate(siblingTable->at(getMaxNumSiblings() - 1), true);
            }

            updateTooltip();

            // lost last sibling?
            if (siblingTable->size() == 0) {
                join();
                return true;
            }

            BUCKET_CONSISTENCY(routingTimeout: is sibling);

            // try to refill with new closest contact
            refillSiblingTable();

            return true;
        }
    }

    /* check if node is already in a bucket ---------------------------------*/
    KademliaBucket* bucket = routingBucket(key, false);
    if (bucket != NULL && (i = bucket->findIterator(key) ) != bucket->end() ) {

        i->incStaleCount();
        std::map<NodeHandle, NodeHandle>::iterator it
                    = replacementCache.find(*i);
        if (i->getStaleCount() > maxStaleCount ||
            it != replacementCache.end() || immediately) {
            // remove from routing table
            bucket->erase(i);
        }
        if (it != replacementCache.end()) {
            routingAdd(it->second, true);
            nodesReplaced++;
            //EV << "node replaced" << endl;
        }
        BUCKET_CONSISTENCY(routingTimeout: is in bucket);
        return true;
    }
    BUCKET_CONSISTENCY(routingTimeout: end);
    return false;
}

void Kademlia::refillSiblingTable()
{
    if (siblingTable->size() == 0 ||
        siblingTable->isFull())
        return;

    int index = routingBucketIndex(siblingTable->back().getKey()) - 1;
    assert(index > 0);

    while ((routingTable[index] == NULL ||
            routingTable[index]->empty()) &&
            index < (int)(OverlayKey::getLength() - 1)) {
        index++;
    }
    if (index < (int)OverlayKey::getLength() &&
            routingTable[index] != NULL && routingTable[index]->size()) {
        KademliaBucket sortedBucket(k, comparator);
        for (uint32_t i = 0; i < routingTable[index]->size(); ++i)
            sortedBucket.add(routingTable[index]->at(i));
        siblingTable->add(sortedBucket.front());
        // no need to callUpdate(), because s < siblingTable->size(), thus
        // new sibling table entry is no real sibling
        routingTable[index]->
        erase(routingTable[index]->
              findIterator(sortedBucket.front().getKey()));
        assert(siblingTable->isFull());
        BUCKET_CONSISTENCY(routingTimeout: end refillSiblingTable());
    }
}

void Kademlia::setBucketUsage(const OverlayKey& key)
{
    KademliaBucket* bucket = routingBucket(key, true);

    if (bucket)
        bucket->setLastUsage(simTime());

/*
    if (!siblingTable->size() || ((siblingTable->back().getKey() ^ thisNode.getKey()) >=
                                  (key ^ thisNode.getKey())))
        siblingTable->setLastUsage(simTime());
 */

    if (((siblingTable->size() + 1) < (uint32_t)getMaxNumSiblings())
        || ((siblingTable->at(getMaxNumSiblings() - 2).getKey() ^ thisNode.getKey())
                >= (key ^ thisNode.getKey()))) {
        siblingTable->setLastUsage(simTime());
    }

}

bool Kademlia::isSiblingFor(const NodeHandle& node, const OverlayKey& key,
                            int numSiblings, bool* err)
{
    if (key.isUnspecified())
        error("Kademlia::isSiblingFor(): key is unspecified!");

    if (state != READY) {
        *err = true;
        return false;
    }

    if (numSiblings > getMaxNumSiblings()) {
        opp_error("Kademlia::isSiblingFor(): numSiblings too big!");
    }

    // set default number of siblings to consider
    if (numSiblings == -1) {
        numSiblings = getMaxNumSiblings();
    }

    if (numSiblings == 0) {
        *err = false;
        return (node.getKey() == key);
    }

    if (siblingTable->size() < (uint)numSiblings) {
        *err = false;
        return true;
    }

    if ((thisNode.getKey() ^ key) > (thisNode.getKey() ^ siblingTable->back().getKey())) {
        *err = true;
        return false;
    }

    KeyDistanceComparator<KeyXorMetric>* comp =
        new KeyDistanceComparator<KeyXorMetric>(key);

    // create result vector
    NodeVector* result = new NodeVector(numSiblings, comp);

    for (KademliaBucket::iterator i=siblingTable->begin();
         i != siblingTable->end(); i++) {
        result->add( *i);
    }

    // add local node
    result->add(thisNode);

    *err = false;
    delete comp;

    if (result->contains(node.getKey())) {
        delete result;
        return true;
    } else {
        delete result;
        assert(!(numSiblings == 1 && key == node.getKey()));
        return false;
    }
}

bool Kademlia::handleFailedNode(const TransportAddress& failed)
{
    assert(!failed.isUnspecified());

    KademliaBucket::iterator i;
    // check sibling table
    for (i = siblingTable->begin(); i != siblingTable->end(); ++i) {
        if (failed == *i) break;
    }

    if (i != siblingTable->end()) {
        // remove from sibling table
        NodeHandle oldSibling = *i;
        siblingTable->erase(i);

        if (siblingTable->size() < (uint32_t)getMaxNumSiblings()) {
            // no new replacement sibling
            deleteOverlayNeighborArrow(oldSibling);
            callUpdate(oldSibling, false);
        } else if (comparator->compare(oldSibling.getKey(),
                           siblingTable->at(getMaxNumSiblings() - 1).getKey()) < 0) {
            // failed sibling was replaced by next closest node in siblingTable
            deleteOverlayNeighborArrow(oldSibling);
            callUpdate(oldSibling, false);

            showOverlayNeighborArrow(siblingTable->at(getMaxNumSiblings() - 1),
                                     false, "m=m,50,100,50,100;ls=green,1");
            callUpdate(siblingTable->at(getMaxNumSiblings() - 1), true);
        }

        updateTooltip();

        // try to refill with new closest contact
        refillSiblingTable();
    } else {
        // check buckets
        uint32_t m;
        for (m = 0; m < routingTable.size(); ++m) {
            if (routingTable[m] != NULL) {
                for (i = routingTable[m]->begin(); i != routingTable[m]->end();
                     ++i) {
                    if (failed == *i) {
                        // remove from routing table
                        routingTable[m]->erase(i);
                        return (siblingTable->size() != 0);
                    }
                }
            }
        }
    }
    return (siblingTable->size() != 0);
}

NodeVector* Kademlia::findNode(const OverlayKey& key, int numRedundantNodes,
                               int numSiblings, BaseOverlayMessage* msg)
{
    if ((numRedundantNodes > getMaxNumRedundantNodes()) ||
            (numSiblings > getMaxNumSiblings())) {

        opp_error("(Kademlia::findNode()) numRedundantNodes or numSiblings "
                  "too big!");
    }

    if (numRedundantNodes < 2) {
        throw cRuntimeError("Kademlia::findNode(): For Kademlia "
                                "redundantNodes must be at least 2 "
                                "and lookupMerge should be true!");
    }

    // create temporary comparator
    KeyDistanceComparator<KeyXorMetric>* comp =
        new KeyDistanceComparator<KeyXorMetric>( key );

    // select result set size
    bool err;
    int resultSize;

    if (numSiblings < 0) {
        // exhaustive iterative doesn't care about siblings
        resultSize = numRedundantNodes;
    } else {
        resultSize = isSiblingFor(thisNode, key, numSiblings, &err) ?
                (numSiblings ? numSiblings : 1) : numRedundantNodes;
    }
    assert(numSiblings || numRedundantNodes);

    NodeVector* result = new NodeVector(resultSize, comp);

    if (siblingTable->isEmpty()) {
        result->add(thisNode);
        delete comp;
        return result;
    }

    // add items from buckets
    int index;
    int mainIndex = routingBucketIndex(key);
    int startIndex = routingBucketIndex(key, true);
    int endIndex = routingBucketIndex(siblingTable->back().getKey());

    // add nodes from best fitting bucket
    if (mainIndex != -1) {
        KademliaBucket* bucket = routingTable[mainIndex];
        if (bucket != NULL && bucket->size()) {
            for (KademliaBucket::iterator i=bucket->begin(); i!=bucket->end(); i++) {
                result->add(*i);
                //EV << "Kademlia::findNode(): Adding "
                //   << *i << " from bucket " << mainIndex << endl;
            }
        }
    }

    // add most fitting buckets
    if (startIndex >= endIndex || !result->isFull()) {
        for (index = startIndex; index >= endIndex; --index) {
            // old code...
            /*
            for (int i = 1; !result->isFull() && i < numBuckets * 3; i++) {
                int index = mainIndex + (((i & 1) == 1) ? -1 : 1) * (i / 2);
                if (index < 0 || index >= numBuckets)
                    continue;
            */

            // add bucket to result vector
            if (index == mainIndex) continue;
            KademliaBucket* bucket = routingTable[index];
            if (bucket != NULL && bucket->size()) {
                for (KademliaBucket::iterator i=bucket->begin(); i!=bucket->end(); i++) {
                    result->add(*i);
                    //EV << "Kademlia::routingGetClosestNodes(): Adding "
                    //   << *i << " from bucket " << index << endl;
                }
            }
        }

        // add nodes from sibling table
        for (KademliaBucket::iterator i = siblingTable->begin();
             i != siblingTable->end(); i++) {
            result->add(*i);
        }
        // add local node
        result->add(thisNode);
    }

    // add more distant buckets
    for (index = mainIndex + 1; !result->isFull() && index < numBuckets;
         ++index) {
        // add bucket to result vector
        KademliaBucket* bucket = routingTable[index];
        if (bucket != NULL && bucket->size()) {
            for (KademliaBucket::iterator i=bucket->begin(); i!=bucket->end(); i++) {
                result->add(*i);
                //EV << "[Kademlia::routingGetClosestNodes()]\n"
                //   << "    Adding " << *i << " from bucket " << index
                //   << endl;
            }
        }
    }

    delete comp;

    return result;
}

//-----------------------------------------------------------------------------


void Kademlia::handleTimerEvent(cMessage* msg)
{
    if (msg == bucketRefreshTimer) {
        handleBucketRefreshTimerExpired();
    }
}

//In Kademlia this method is used to maintain the routing table.
bool Kademlia::handleRpcCall(BaseCallMessage* msg)
{
    RPC_SWITCH_START(msg)
    RPC_ON_CALL(Ping) {
        // add active node
        OverlayCtrlInfo* ctrlInfo =
            check_and_cast<OverlayCtrlInfo*>(msg->getControlInfo());
        routingAdd(ctrlInfo->getSrcRoute(), true );
        break;
    }
    RPC_ON_CALL(FindNode)
    {
        // add active node
        OverlayCtrlInfo* ctrlInfo =
            check_and_cast<OverlayCtrlInfo*>(msg->getControlInfo());
        routingAdd(ctrlInfo->getSrcRoute(), true);
        break;
    }
    RPC_SWITCH_END()
    return false;
}

//In Kademlia this method is used to maintain the routing table.
void Kademlia::handleRpcResponse(BaseResponseMessage* msg,
                                 cPolymorphic* context, int rpcId,
                                 simtime_t rtt)
{
    OverlayCtrlInfo* ctrlInfo =
        dynamic_cast<OverlayCtrlInfo*>(msg->getControlInfo());
    NodeHandle srcRoute = (ctrlInfo ? ctrlInfo->getSrcRoute()
                                    : msg->getSrcNode());

    RPC_SWITCH_START(msg)
        RPC_ON_RESPONSE(Ping) {
            if (state == INIT) {
                // schedule bucket refresh timer
                cancelEvent(bucketRefreshTimer);
                scheduleAt(simTime(), bucketRefreshTimer);
                state = JOIN;
            }

            // delete replacement candidate //TODO
            replacementCache.erase(srcRoute);
        }
        RPC_ON_RESPONSE(FindNode)
        {
            // add active node
            if (defaultRoutingType == SEMI_RECURSIVE_ROUTING ||
                defaultRoutingType == FULL_RECURSIVE_ROUTING ||
                defaultRoutingType == RECURSIVE_SOURCE_ROUTING) {
                rtt = MAXTIME;
            }
            setBucketUsage(srcRoute.getKey());

            // add inactive nodes
            for (uint32_t i=0; i<_FindNodeResponse->getClosestNodesArraySize(); i++)
                routingAdd(_FindNodeResponse->getClosestNodes(i), false);
            break;
        }
    RPC_SWITCH_END()

    // add node that responded
    routingAdd(srcRoute, true, rtt);
}

// In Kademlia this method is used to maintain the routing table.
void Kademlia::handleRpcTimeout(BaseCallMessage* msg,
                                const TransportAddress& dest,
                                cPolymorphic* context, int rpcId,
                                const OverlayKey& destKey)
{
    if (dest.isUnspecified()) return;
    try {
        RPC_SWITCH_START(msg)
        RPC_ON_CALL(Ping) {
            if (state == INIT) {
                join();
                return;
            }

            const NodeHandle& handle = dynamic_cast<const NodeHandle&>(dest);
            routingTimeout(handle.getKey());

            // remove node from replacementCache
            std::map<NodeHandle, NodeHandle>::iterator it
                = replacementCache.find(handle);
            if (it != replacementCache.end()) {
                replacementCache.erase(it);
            }

            break;
        }
        RPC_ON_CALL(FindNode) {
            const NodeHandle& handle = dynamic_cast<const NodeHandle&>(dest);
            routingTimeout(handle.getKey());
            setBucketUsage(handle.getKey());
            break;
        }
        RPC_SWITCH_END()
    } catch (...) {
        EV << "[Kademlia:handleRpcTimout() @ " << thisNode.getAddress()
           << " (" << thisNode.getKey().toString(16) << ")]\n"
           << "    ERROR: RPC timeout without key ("
           << msg << " -> " << dest << ")" << endl;
        return;
    }
}

void Kademlia::lookupFinished(bool isValid)
{
    if (state == JOIN) {
        cancelEvent(bucketRefreshTimer);

        if (siblingTable->size() == 0) {
            // initial lookup failed - get new bootstrap node
            join();
            return;
        }

        scheduleAt(simTime(), bucketRefreshTimer);

        state = READY;
        setOverlayReady(true);
    }
}

// handle a expired bucket refresh timer
void Kademlia::handleBucketRefreshTimerExpired()
{
    // refresh buckets
    if (state != READY || (((simTime() - siblingTable->getLastUsage()) >
                            minSiblingTableRefreshInterval))) {
        if (exhaustiveRefresh) {
            // TODO: replace redundantNodes by k?
            createLookup(EXHAUSTIVE_ITERATIVE_ROUTING)->lookup(
                 getThisNode().getKey(), iterativeLookupConfig.redundantNodes,
                 0, 0, new KademliaLookupListener(this));
        } else {
            createLookup()->lookup(getThisNode().getKey(), s, 0, 0,
                                   new KademliaLookupListener(this));
        }
        ++siblingTableRefreshCount;
    }

    if (state == READY) {
        if (siblingTable->size()) {
            // get bit index of most significant digit that differs
            // from our next sibling's key to prevent us from refreshing
            // buckets, which can't contain any nodes
            int32_t diff = OverlayKey::getLength() - b*(getThisNode().getKey().
                sharedPrefixLength(siblingTable->front().getKey(), b) + 1);
            int bucketsRefreshedPerTask = 0;
            for (int32_t i = OverlayKey::getLength() - b; i >= diff; i -=b ) {
                for (int32_t d=0; d < ((1 << b) - 1); d++) {
                    int32_t index = (i / b) * ((1 << b) - 1) + d;
                    if ((routingTable[index] == NULL) ||
                        ((simTime() - routingTable[index]->getLastUsage()) >
                        minBucketRefreshInterval)) {

                        OverlayKey refreshKey =
                            getThisNode().getKey() ^ (OverlayKey(d+1) << i);

                        if (exhaustiveRefresh) {
                            createLookup(EXHAUSTIVE_ITERATIVE_ROUTING)->lookup(
                                refreshKey, iterativeLookupConfig.redundantNodes,
                                0, 0, new KademliaLookupListener(this));
                        } else {
                            createLookup()->lookup(refreshKey, s, 0, 0,
                                             new KademliaLookupListener(this));
                        }

                        ++bucketsRefreshedPerTask;
                        ++bucketRefreshCount;
                        setBucketUsage(refreshKey);
                    }
                }
            }
            RECORD_STATS(globalStatistics->recordOutVector(
                                   "Kademlia: Buckets Refreshed Per Task",
                                   bucketsRefreshedPerTask));
        }
        // schedule next bucket refresh process
        cancelEvent(bucketRefreshTimer);
        scheduleAt(simTime() + (std::min(minSiblingTableRefreshInterval,
                        minBucketRefreshInterval) / 10.0), bucketRefreshTimer);
    }
}

//virtual public: xor metric
OverlayKey Kademlia::distance(const OverlayKey& x, const OverlayKey& y) const
{
    return x^y;
}

void Kademlia::updateTooltip()
{
    if (ev.isGUI()) {
        std::stringstream ttString;

        // show our nodeId in a tooltip
        ttString << "This: " << thisNode << endl << "Siblings: "
                 << siblingTable->size();

        getParentModule()->getParentModule()->getDisplayString().
        setTagArg("tt", 0, ttString.str().c_str());
        getParentModule()->getDisplayString().
        setTagArg("tt", 0, ttString.str().c_str());
        getDisplayString().setTagArg("tt", 0, ttString.str().c_str());
    }
}

