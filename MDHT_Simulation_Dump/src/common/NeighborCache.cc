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
//

/**
 * @file NeighborCache.cc
 * @author Antonio Zea
 * @author Bernhard Heep
 */

#include <cassert>

#include <TransportAddress.h>
#include <NodeHandle.h>
#include <PeerInfo.h>
#include <GlobalStatisticsAccess.h>
#include <Vivaldi.h>
#include <CoordMessages_m.h>
#include <GlobalNodeListAccess.h>
#include <hashWatch.h>

#include "NeighborCache.h"


const std::vector<double> NeighborCache::coordsDummy;

std::ostream& operator<<(std::ostream& os,
                         const NeighborCache::NeighborCacheEntry& entry)
{
    if (entry.rttState == NeighborCache::RTTSTATE_VALID) {
        os << entry.rtt;
    } else {
        if (entry.rttState == NeighborCache::RTTSTATE_TIMEOUT) os << "TIMEOUT";
        else if (entry.rttState == NeighborCache::RTTSTATE_UNKNOWN) os << "UNKNOWN";
        else if (entry.rttState == NeighborCache::RTTSTATE_WAITING) os << "WAITING";
    }
    os << " (inserted: " << entry.insertTime;

    if (entry.waitingContexts != NULL) {
        os << ", #contexts: "
           << entry.waitingContexts->pingContexts.size();
    }
    if (!entry.nodeRef.isUnspecified()) os <<  ", <KEY>";

    if (entry.coordsInfo.coordinates.size() > 0) {
        if (entry.coordsInfo.npsLayer >= 0) {
            os << ", NPS layer: "
               << (int)entry.coordsInfo.npsLayer;
        }
        os << ", coords: <";
        for (uint32_t i = 0; i < entry.coordsInfo.coordinates.size(); ++i) {
            if (i != 0) os << ", ";
            os << entry.coordsInfo.coordinates[i];
        }
        if (entry.coordsInfo.coordErr >= 0) {
            os << ", E: "
               << entry.coordsInfo.coordErr;
        }
        os << ">";
    }
    os << ")";

    return os;
}


Define_Module(NeighborCache);

void NeighborCache::initializeApp(int stage)
{
    if (stage != MIN_STAGE_COMPONENTS) {
            return;
    }

    neighborCache.clear();
    //WATCH_MAP(neighborCache);
    WATCH_UNORDERED_MAP(neighborCache);

    enableNeighborCache = par("enableNeighborCache");
    rttExpirationTime = par("rttExpirationTime");
    maxSize = par("maxSize");

    // set default query types
    std::string temp = par("defaultQueryType").stdstringValue();
    if (temp == "exact")
        defaultQueryType = NEIGHBORCACHE_EXACT;
    else if (temp == "exact_timeout")
        defaultQueryType = NEIGHBORCACHE_EXACT_TIMEOUT;
    else if (temp == "available")
        defaultQueryType = NEIGHBORCACHE_AVAILABLE;
    else if (temp == "estimated")
        defaultQueryType = NEIGHBORCACHE_ESTIMATED;
    else throw cRuntimeError((std::string("Wrong query type: ")
                             + temp).c_str());

    temp = par("defaultQueryTypeI").stdstringValue();
    if (temp == "available")
        defaultQueryTypeI = NEIGHBORCACHE_AVAILABLE;
    else if (temp == "estimated")
        defaultQueryTypeI = NEIGHBORCACHE_ESTIMATED;
    else throw cRuntimeError((std::string("Wrong query type (I): ")
                             + temp).c_str());

    temp = par("defaultQueryTypeQ").stdstringValue();
    if (temp == "exact")
        defaultQueryTypeQ = NEIGHBORCACHE_EXACT;
    else if (temp == "exact_timeout")
        defaultQueryTypeQ = NEIGHBORCACHE_EXACT_TIMEOUT;
    else if (temp == "query")
        defaultQueryTypeQ = NEIGHBORCACHE_QUERY;
    else throw cRuntimeError((std::string("Wrong query type (Q): ")
                             + temp).c_str());

    // nps/gnp init
    enableNps = par("enableNps");
    if (enableNps) {
        nps = new Nps();
        nps->initNps(this);
        //TODO joinOnApplicationRequest
    } else {
        nps = NULL;
    }

    // vivaldi init
    enableVivaldi = par("enableVivaldi");
    if (enableVivaldi) {
        vivaldi = new Vivaldi();
        vivaldi->initVivaldi(this);
    } else {
        vivaldi = NULL;
    }

    globalStatistics = GlobalStatisticsAccess().get();

    misses = 0;
    hits = 0;

    //TODO
    //-------
    rttErrorHistory = par("rttErrorHistory");
    rttHistory = par("rttHistory");
    numMsg = 0;
    absoluteError = 0.0;
    relativeError = 0.0;
    numRttErrorToHigh = 0;
    numRttErrorToLow = 0;
    lastAbsoluteErrorPerNode.clear();
    WATCH(absoluteError);
    WATCH(relativeError);
    WATCH(numMsg);

    /*
    countGetProxTotal = 0.0;
    countGetProxSuccessful = 0.0;
    relativeGetProx = 0.0;

    countCheckEntryTotal = 0.0;
    countCheckEntrySuccessful = 0.0;
    relativeCheckEntry = 0.0;
    */
    //-------

    //lastProxRpcId = 0;
    //proxCacheListener.parent = this;

}

void NeighborCache::finishApp()
{
    if ((misses + hits) != 0) {
        globalStatistics
            ->addStdDev("NeighborCache: Ping hit rate",
                        ((double)hits / (double)(misses + hits)));
    }

    //TODO
    //---------
    if (numMsg > 0 && (enableVivaldi || enableNps)) {
        globalStatistics->addStdDev("NeighborCache: absolute RTT error",
                                    absoluteError / numMsg);
        globalStatistics->addStdDev("NeighborCache: relative RTT error",
                                    relativeError / numMsg);
        globalStatistics->addStdDev("NeighborCache: number of messages/s",
                                    numMsg / SIMTIME_DBL(simTime() - creationTime));
        globalStatistics->addStdDev("NeighborCache: percentage of RTT errors to high",
                                    (double)numRttErrorToHigh / (double)numMsg);
        globalStatistics->addStdDev("NeighborCache: percentage of RTT errors to low",
                                    (double)numRttErrorToLow / (double)numMsg);
    }
//    globalStatistics->addStdDev("NeighborCache: percentage of Successful getProx operation",
//                                relativeGetProx);
//    globalStatistics->addStdDev("NeighborCache: total getProx operation",
//                                countGetProxTotal);
//    globalStatistics->addStdDev("NeighborCache: Successful getProx operation",
//                                countGetProxSuccessful);
//    globalStatistics->addStdDev("NeighborCache: Successful checkEntry operation",
//                                countCheckEntrySuccessful);
//    globalStatistics->addStdDev("NeighborCache: total checkEntry operation",
//                                countCheckEntryTotal);
//    globalStatistics->addStdDev("NeighborCache: percentage of Successful checkEntry operation",
//                                relativeCheckEntry);
    //---------
}


NeighborCache::~NeighborCache()
{
    delete nps;
    delete vivaldi;

    for (NeighborCacheIterator it = neighborCache.begin();
         it != neighborCache.end(); ++it) {
        if (it->second.waitingContexts) {
            for (uint32_t i = 0;
                 i < it->second.waitingContexts->pingContexts.size(); ++i) {
                delete it->second.waitingContexts->pingContexts[i];
            }

            delete it->second.waitingContexts;
        }
    }
}

bool NeighborCache::insertNodeContext(const TransportAddress& handle,
                                     cPolymorphic* context,
                                     ProxListener* rpcListener,
                                     int rpcId)
{
    if (!enableNeighborCache) return false;
    if (neighborCache.count(handle) == 0) {
        NeighborCacheEntry entry;

        entry.insertTime = simTime();
        entry.rttState = RTTSTATE_WAITING;
        entry.nodeRef = NodeHandle::UNSPECIFIED_NODE;
        entry.coordsInfo.coordErr = 1;
        entry.coordsInfo.npsLayer = -1;
        entry.rttCounter = 0;

        neighborCache[handle] = entry;
        neighborCacheExpireMap.insert(std::make_pair(entry.insertTime,
                                                     handle));

        cleanupCache();

        assert(neighborCache.size() == neighborCacheExpireMap.size());
        return false;
    } else {
        NeighborCacheEntry& entry = neighborCache[handle];

        // waiting?
        if (entry.rttState == RTTSTATE_WAITING) {
            assert(entry.proxContext != NULL);
            if (entry.waitingContexts == NULL)
                entry.waitingContexts = new WaitingContexts();
            entry.waitingContexts->pingContexts.push_back(context);
            entry.waitingContexts->pingListeners.push_back(rpcListener);
            entry.waitingContexts->pingIds.push_back(rpcId);

            return true;
        } else {
            if (entry.waitingContexts != NULL &&
                entry.waitingContexts->pingContexts.size() != 0) {
                throw cRuntimeError("not waiting for response,"
                                    " but additional contexts found!");
            }

            updateEntry(handle, entry.insertTime);

            assert(entry.waitingContexts == NULL && entry.proxContext == NULL);

            entry.rttState = RTTSTATE_WAITING;
            entry.insertTime = simTime();
            entry.nodeRef = NodeHandle::UNSPECIFIED_NODE;

            return false;
        }
    }
}


NeighborCache::WaitingContexts* NeighborCache::getNodeContexts(const TransportAddress& handle)
{
    if (!enableNeighborCache) {
        assert(neighborCache.count(handle) == 0 ||
               neighborCache[handle].waitingContexts == NULL);
        return NULL;
    }
    if (neighborCache.count(handle) == 0)
        throw cRuntimeError("NeighborCache error!");
    WaitingContexts* temp = neighborCache[handle].waitingContexts;
    neighborCache[handle].waitingContexts = NULL;

    return temp;
}


void NeighborCache::setNodeTimeout(const TransportAddress& handle)
{
    if (!enableNeighborCache) return;

    if (neighborCache.count(handle) == 0) {
        NeighborCacheEntry entry;

        entry.insertTime = simTime();
        entry.rttState = RTTSTATE_TIMEOUT;
        entry.nodeRef = NodeHandle::UNSPECIFIED_NODE;
        entry.coordsInfo.npsLayer = -1;
        entry.coordsInfo.coordErr = -1;
        entry.rttCounter = 0;

        neighborCache[handle] = entry;
        neighborCacheExpireMap.insert(std::make_pair(entry.insertTime,
                                                     handle));
        cleanupCache();
    } else {
        NeighborCacheEntry& entry = neighborCache[handle];

        assert(!(entry.proxContext == NULL && entry.waitingContexts != NULL));

        updateEntry(handle, entry.insertTime);

        entry.insertTime = simTime();
        entry.rttState = RTTSTATE_TIMEOUT;

        if (entry.proxContext) {
            WaitingContexts* waitingContexts = getNodeContexts(handle);

            ProxContext* context = entry.proxContext;
            entry.proxContext = NULL;

            if (context && context->listener) {
                context->listener->proxCallback(handle, context->rpcId,
                                                context->contextPointer,
                                                Prox::PROX_TIMEOUT);

                if (waitingContexts) {
                    for (uint32_t i = 0; i < waitingContexts->size(); ++i) {
                        if (waitingContexts->pingListeners[i]) {
                            waitingContexts->pingListeners[i]
                                ->proxCallback(handle,
                                               waitingContexts->pingIds[i],
                                               waitingContexts->pingContexts[i],
                                               Prox::PROX_TIMEOUT);
                        }
                    }
                }
            }
            delete context;
            delete waitingContexts;
        }
    }
    assert(neighborCache.size() == neighborCacheExpireMap.size());
}


void NeighborCache::updateNode(const NodeHandle& add, simtime_t rtt,
                               const NodeHandle& srcRoute,
                               const std::vector<double>& coords,
                               double error)
{
    EV << "[NeighborCache::updateNode() @ " << thisNode.getAddress()
           << " (" << thisNode.getKey().toString(16) << ")]\n"
           << "    inserting rtt(" << rtt << ") of node " << add.getAddress()
           << endl;

    if (rtt <= 0) return; //TODO broose

    if (enableNeighborCache) {
        if (neighborCache.count(add) == 0) {
            NeighborCacheEntry entry;

            entry.insertTime = simTime();
            entry.rtt = rtt;
            entry.rttState = RTTSTATE_VALID;
            entry.nodeRef = add;
            entry.coordsInfo.npsLayer = -1;
            entry.coordsInfo.coordinates = coords;
            entry.coordsInfo.coordErr = error;
            entry.lastRtts.push_back(rtt);
            entry.rttCounter = 1;

            neighborCache[add] = entry;
            neighborCacheExpireMap.insert(std::make_pair(entry.insertTime,
                                                         add));

            cleanupCache();
        } else {
            updateEntry(add, neighborCache[add].insertTime);

            NeighborCacheEntry& entry = neighborCache[add];

            entry.insertTime = simTime();
            if (entry.rttState != RTTSTATE_VALID || entry.rtt > rtt)
                entry.rtt = rtt;
            entry.rttState = RTTSTATE_VALID;
            entry.nodeRef = add;

            if (entry.rttCounter < rttHistory) {
                entry.lastRtts.push_back(rtt);
            } else {
                entry.lastRtts.at((entry.rttCounter % rttHistory)) = rtt;
            }

            entry.rttCounter += 1;

            if (entry.coordsInfo.coordErr > error) { //TODO
                entry.coordsInfo.coordinates = coords;
                entry.coordsInfo.coordErr = error;
            }

            if (entry.proxContext) {
                WaitingContexts* waitingContexts = getNodeContexts(add);

                ProxContext* context = entry.proxContext;
                entry.proxContext = NULL;

                if (context && context->listener) {
                    context->listener->proxCallback(add, context->rpcId,
                                                    context->contextPointer,
                                                    Prox(rtt, 1));

                    if (waitingContexts) {
                        for (uint32_t i = 0; i < waitingContexts->size(); ++i) {
                            if (waitingContexts->pingListeners[i]) {
                                waitingContexts->pingListeners[i]
                                    ->proxCallback(add,
                                                   waitingContexts->pingIds[i],
                                                   waitingContexts->pingContexts[i],
                                                   Prox(rtt, 1));
                            }
                        }
                    }
                }
                delete context;
                delete waitingContexts;
            }
            assert(entry.proxContext == NULL && entry.waitingContexts == NULL);
        }
        assert(neighborCache.size() == neighborCacheExpireMap.size());
    }
    //TODO
    //--------
    calcRttError(add, rtt);
    //--------
    if (enableVivaldi) vivaldi->processCoordinates(rtt, coords, error);
}

//TODO duplicated code
void NeighborCache::setNodeLayer(const NodeHandle& add, int8_t layer)
{
    if (!enableNeighborCache) return;

    if (neighborCache.count(add) == 0) {
        NeighborCacheEntry entry;

        entry.insertTime = simTime();
        entry.nodeRef = add;
        entry.coordsInfo.npsLayer = layer;
        entry.coordsInfo.coordErr = 1;
        entry.rttCounter = 0;

        neighborCache[add] = entry;
        neighborCacheExpireMap.insert(std::make_pair(entry.insertTime, add));

        cleanupCache();
    } else {
        updateEntry(add, neighborCache[add].insertTime);

        neighborCache[add].insertTime = simTime();
        neighborCache[add].nodeRef = add;
        neighborCache[add].coordsInfo.npsLayer = layer;
    }
}


NeighborCache::Rtt NeighborCache::getNodeRtt(const TransportAddress &add)
{
    // cache disabled or entry not there
    if (!enableNeighborCache ||
        add.isUnspecified() ||
        (neighborCache.count(add) == 0)) {
        misses++;
        return std::make_pair(0.0, RTTSTATE_UNKNOWN);
    }

    NeighborCacheEntry &entry = neighborCache[add];

    if (entry.rttState == RTTSTATE_WAITING ||
        entry.rttState == RTTSTATE_UNKNOWN)
        return std::make_pair(entry.rtt, entry.rttState);
    // entry expired
    if ((simTime() - entry.insertTime) >= rttExpirationTime) {
        assert(entry.waitingContexts == NULL);
        entry.rttState = RTTSTATE_UNKNOWN;
        return std::make_pair(entry.rtt, RTTSTATE_UNKNOWN);
    }
    hits++;
    assert(!(entry.rtt == 0.0 && entry.rttState == RTTSTATE_VALID));
    return std::make_pair(entry.rtt, entry.rttState);
}


const NodeHandle& NeighborCache::getNodeHandle(const TransportAddress &add)
{
    if (neighborCache.count(add) == 0) {
        throw cRuntimeError("NeighborCache.cc: getNodeHandle was asked for "
                            "a non-existent node reference.");
    }
    return neighborCache[add].nodeRef;
}

bool NeighborCache::cleanupCache()
{
    bool result = false;
    uint32_t size = neighborCache.size();

    if (size > maxSize) {
        neighborCacheExpireMapIterator it;
        for (uint32_t i = 0; i < (size - (maxSize / 2)); ++i) {
            it = neighborCacheExpireMap.begin();
            if ((neighborCache[it->second].rttState == RTTSTATE_WAITING) ||
                (neighborCache[it->second].insertTime == simTime())) {
                break;
            }
            neighborCache.erase(it->second);
            neighborCacheExpireMap.erase(it);
            result = true;
        }
    }
    assert(neighborCache.size() == neighborCacheExpireMap.size());
    return result;
}

void NeighborCache::updateEntry(const TransportAddress& address,
                                simtime_t insertTime)
{
    neighborCacheExpireMapIterator it =
        neighborCacheExpireMap.lower_bound(insertTime);
    while (it->second != address) ++it;
    neighborCacheExpireMap.erase(it);
    neighborCacheExpireMap.insert(std::make_pair(simTime(),
                                                 address));
    assert(neighborCache.size() == neighborCacheExpireMap.size());
}

// coordinate stuff
std::vector<LandmarkDataEntry> NeighborCache::getLandmarkData(const std::vector<TransportAddress>& landmarkSet) const
{
    std::vector<LandmarkDataEntry> returnVector;
    NeighborCacheConstIterator it;

    for(it = neighborCache.begin(); it != neighborCache.end(); it++ ) {
        for (uint i = 0; i < landmarkSet.size(); i++) {
            if (it->first == landmarkSet[i]) {
                // get the relevant data of this landmark.
                LandmarkDataEntry thisEntry =
                    { it->second.rtt,
                      it->second.coordsInfo.npsLayer,
                      it->second.coordsInfo.coordinates,
                      &it->first.getAddress() };
                returnVector.push_back(thisEntry);
            }
        }
    }
    return returnVector;
}

bool NeighborCache::setLandmarkSet(uint8_t howManyLM, uint8_t maxLayer,
                                   std::vector<TransportAddress>* landmarkSet)
{
    landmarkSet->clear();
    NeighborCacheIterator it;
    uint availableLM = 0;
    TransportAddress landmark;
    for(it = neighborCache.begin(); it != neighborCache.end(); it++ ) {
        landmark.setAddress(it->first.getAddress());
        landmark.setPort(it->second.nodeRef.getPort());
        if (it->second.coordsInfo.npsLayer < maxLayer) {
            availableLM++;
            landmarkSet->push_back(landmark);
        }
    }
    if (availableLM < howManyLM) {
        return false;
    } else {
        uint i = availableLM;
        while (i > howManyLM) {
            uint randomNumber = (intuniform(0, landmarkSet->size()));
            landmarkSet->erase(landmarkSet->begin() + randomNumber);
            i--;
        }
        return true;
    }
}


TransportAddress NeighborCache::getNearestNode(uint8_t maxLayer)
{
    TransportAddress nearestNode = TransportAddress::UNSPECIFIED_NODE;
    simtime_t nearestNodeRtt = MAXTIME;
    NeighborCacheIterator it;
    if (neighborCache.size() > 1) {
        for(it = neighborCache.begin(); it != neighborCache.end(); it++ ) {
            if (it->second.rtt < nearestNodeRtt &&
                it->second.rtt > 0 &&
                it->second.coordsInfo.npsLayer < maxLayer+1 &&
                it->second.coordsInfo.npsLayer > 0) {
                nearestNode.setAddress(it->first.getAddress());
                nearestNodeRtt = it->second.rtt;
                nearestNode.setPort(it->second.nodeRef.getPort());
            }
        }
    }
    return nearestNode;
}

bool NeighborCache::isEntry(const TransportAddress &node)
{
    NeighborCacheIterator it;
    for(it = neighborCache.begin(); it != neighborCache.end(); it++ ) { //TODO find
        if (it->first == node) {
            return true;
        }
    }
    return false;
}

// Vivaldi stuff
double NeighborCache::getAvgAbsPredictionError(std::vector<double>& coord,
                                               uint32_t sampleSize)
{
    //TODO version roth
    double absoluteDiff = 0.0;
    uint32_t numNeighbors = 0;
    uint32_t i;

    for (std::map<TransportAddress, std::vector<double> >::iterator it =
        lastAbsoluteErrorPerNode.begin(); it != lastAbsoluteErrorPerNode.end();
        it++) {
        double tempAbsoluteDiff = 0.0;
        for (i = 0; i < it->second.size(); i++) {
            tempAbsoluteDiff += it->second.at(i);
        }
        absoluteDiff += (tempAbsoluteDiff / it->second.size());
        numNeighbors++;
    }

    absoluteDiff /= numNeighbors;
    return (absoluteDiff > 1.0) ? 1.0 : absoluteDiff;

    // old version
    /*
    if (neighborCache.size() < 2 || sampleSize == 0) return 1.0;

    double absoluteDiff = 0;
    uint32_t numNeighbors = 0;

    //for (std::map<IPvXAddress, NeighborCacheEntry>::iterator it =
    //     neighborCache.begin(); it != neighborCache.end(); ++it) {
    for (std::map<simtime_t, IPvXAddress>::reverse_iterator it =
         neighborCacheExpireMap.rbegin();
         it != neighborCacheExpireMap.rend() &&
         numNeighbors < sampleSize; ++it) {
        NeighborCacheEntry& cacheEntry = neighborCache[it->second];
        double dist = 0;
        for (uint32_t i = 0; i < cacheEntry.coordsInfo.coordinates.size(); i++) {
            dist += pow(coord[i] - cacheEntry.coordsInfo.coordinates[i], 2);
        }
        dist = sqrt(dist);

        if (dist != 0 && cacheEntry.rtt > 0) {
            double predictionError = fabs(dist - SIMTIME_DBL(cacheEntry.rtt));

            //test: error weighted
            //if (it->second.coordErr < 1) {
            //    predictionError /= it->second.coordErr;
            //}
            //test: age weighted
            //if ((simTime() - it->second.insertTime) > 1) {
            //    predictionError /= (simTime() - it->second.insertTime);
            //}

            numNeighbors++;
            absoluteDiff += predictionError;
        }
    }
    assert(numNeighbors != 0);
    absoluteDiff /= numNeighbors;

    return (absoluteDiff > 1.0) ? 1.0 : absoluteDiff;
    */
}


void NeighborCache::handleTimerEvent(cMessage* msg)
{
    if (nps) {
        nps->handleTimerEvent(msg);
    }
}


bool NeighborCache::handleRpcCall(BaseCallMessage* msg)
{
    RPC_SWITCH_START( msg );
        if (nps) {
            RPC_DELEGATE( CoordsReq, nps->coordsReqRpc );
        }
        //if (discovery)
    RPC_SWITCH_END( );

    return RPC_HANDLED;
}



// Prox stuff

Prox NeighborCache::getProx(const TransportAddress &node,
                              NeighborCacheQueryType type,
                              int rpcId,
                              ProxListener *listener,
                              cPolymorphic *contextPointer)
{
    Enter_Method("getProx()");

    if (node == overlay->getThisNode()) return Prox::PROX_SELF;

    bool sendQuery = false;
    Prox result = Prox::PROX_UNKNOWN;
    Rtt rtt = getNodeRtt(node);

    //countGetProxTotal++;
    if (type == NEIGHBORCACHE_DEFAULT) type = defaultQueryType;
    else if (type == NEIGHBORCACHE_DEFAULT_IMMEDIATELY) type = defaultQueryTypeI;
    else if (type == NEIGHBORCACHE_DEFAULT_QUERY) type = defaultQueryTypeQ;

    switch(type) {
        case NEIGHBORCACHE_EXACT:
            if (rtt.second == RTTSTATE_TIMEOUT) {
                // if timeout, return unknown, and send a query!
                sendQuery = true;
            } else if (rtt.second == RTTSTATE_WAITING) {
                // if a query was sent, return UNKNOWN
                sendQuery = true; //just inserting a context, no real ping is sent
            } else if (rtt.second == RTTSTATE_UNKNOWN) {
                // if no entry known, send a query and return UNKNOWN
                sendQuery = true;
            } else {
                // else, return whatever we have
                result = rtt.first;
            }
            break;
        case NEIGHBORCACHE_EXACT_TIMEOUT:
            if (rtt.second == RTTSTATE_TIMEOUT) {
                // if timeout, return that
                result = Prox::PROX_TIMEOUT;
            } else if (rtt.second == RTTSTATE_WAITING) {
                // if a query was sent, return UNKNOWN
                sendQuery = true; //just inserting a context, no real ping is sent
            } else if (rtt.second == RTTSTATE_UNKNOWN) {
                // if no entry known, send a query and return UNKNOWN
                sendQuery = true;
            } else {
                // else, return whatever we have
                result = rtt.first;
            }
            break;
        case NEIGHBORCACHE_ESTIMATED:
            if (rtt.second == RTTSTATE_TIMEOUT) {
                // if timeout, return that
                result = Prox::PROX_TIMEOUT;
            } else if (rtt.second == RTTSTATE_WAITING) {
                // if a query was sent, return an estimate
                result = estimateProx(node);
            } else if (rtt.second == RTTSTATE_UNKNOWN) {
                // if no entry known, return an estimate
                result = estimateProx(node);
            } else {
                // else return whatever we have
                result = rtt.first;
            }
            break;
        case NEIGHBORCACHE_AVAILABLE:
            if (rtt.second == RTTSTATE_TIMEOUT) {
                // if timeout, return that.
                result = Prox::PROX_TIMEOUT;
            } else if ((rtt.second == RTTSTATE_WAITING) ||
                       (rtt.second == RTTSTATE_UNKNOWN)) {
                // if a query was sent or entry unknown, return UNKNOWN
            } else {
                // else return what we have
                result = rtt.first;
            }
            break;
        case NEIGHBORCACHE_QUERY:
            // simply send a query and return UNKNOWN
            sendQuery = true;
            break;
        default:
            throw cRuntimeError("Unknown query type!");
            break;

    }
    if (sendQuery) {
        if (!insertNodeContext(node, contextPointer, listener, rpcId)) {
            queryProx(node, rpcId, listener, contextPointer);
        }
    } else delete contextPointer;

    return result;
}

Prox NeighborCache::estimateProx(const TransportAddress &node)
{
    Enter_Method("estimateProx()");

    Rtt rtt = getNodeRtt(node);

    if (rtt.second != RTTSTATE_UNKNOWN) return rtt.first;

    if ((enableVivaldi || enableNps) && neighborCache.count(node)) {
        return getCoordinateBasedProx(node.getAddress());
    }

    return Prox::PROX_UNKNOWN;
}

void NeighborCache::queryProx(const TransportAddress &node,
                              int rpcId,
                              ProxListener *listener,
                              cPolymorphic *contextPointer)
{
    Enter_Method("queryProx()");

    ProxContext* context = new ProxContext;
    context->rpcId = rpcId;
    context->listener = listener;
    context->contextPointer = contextPointer;

    if (neighborCache.count(node) == 0) {
        NeighborCacheEntry entry;
        entry.proxContext = context;
        neighborCache[node] = entry;
        neighborCacheExpireMap.insert(std::make_pair(entry.insertTime,
                                                     node));
        cleanupCache();
    } else {
        NeighborCacheEntry& entry = neighborCache[node];
        if (entry.proxContext != NULL) throw cRuntimeError("double proxContext");
        entry.proxContext = context;
    }
    assert(neighborCache.size() == neighborCacheExpireMap.size());

    pingNode(node, -1, 0, NULL, "PING");
}

Prox NeighborCache::getCoordinateBasedProx(const TransportAddress& node)
{
    if (enableVivaldi) { //Vivaldi
        return vivaldi->getCoordinateBasedProx(getNodeCoordsInfo(node));
    } else if (enableNps){ //NPS
        return nps->getCoordinateBasedProx(getNodeCoordsInfo(node));
    } else return Prox::PROX_UNKNOWN;
}


const NodeCoordsInfo& NeighborCache::getNodeCoordsInfo(const TransportAddress &node)
{
    if (neighborCache.count(node) == 0) {
        throw cRuntimeError("NeighborCache.cc: getNodeCoords was asked for "
                            "a non-existent node reference.");
    }
    return neighborCache[node].coordsInfo;
}

void NeighborCache::setNodeCoordsInfo(const TransportAddress& handle,
                                      const NodeCoordsInfo& coords)
{
    if (!enableNeighborCache) return;
    if (neighborCache.count(handle) == 0) {
        NeighborCacheEntry entry;

        entry.insertTime = simTime();
        entry.rttState = RTTSTATE_UNKNOWN;
        entry.nodeRef = NodeHandle::UNSPECIFIED_NODE;
        entry.coordsInfo = coords;
        entry.rttCounter = 0;

        neighborCache[handle] = entry;
        neighborCacheExpireMap.insert(std::make_pair(entry.insertTime,
                                                     handle));

        cleanupCache();
    } else {
        NeighborCacheEntry& entry = neighborCache[handle];

        updateEntry(handle, entry.insertTime);

        entry.insertTime = simTime();
        if (entry.coordsInfo.coordErr > coords.coordErr)
            entry.coordsInfo = coords;
    }
    assert(neighborCache.size() == neighborCacheExpireMap.size());
}

//TODO roth
void NeighborCache::calcRttError(const NodeHandle& handle, simtime_t rtt)
{
    if (!enableVivaldi && !enableNps)
        return;

    //calculate absloute rtt error of the last message
    //double tempRttError = SIMTIME_DBL(getNodePredictedRtt(handle) - rtt);
    double tempRttError = getCoordinateBasedProx(handle).proximity - SIMTIME_DBL(rtt);


    lastAbsoluteErrorPerNode[handle].push_back(fabs(tempRttError));
    //      lastAbsoluteErrorPerNode[handle.ip].push_back(SIMTIME_DBL(rtt));
    if(lastAbsoluteErrorPerNode[handle].size() > (uint32_t)rttErrorHistory)
        lastAbsoluteErrorPerNode[handle].erase(lastAbsoluteErrorPerNode[handle].begin());

    // Check if data collection can start
    if(!globalStatistics->isMeasuring()) return;

    if(tempRttError < 0){
        tempRttError *=-1;
        numRttErrorToLow++;
    }else numRttErrorToHigh++;


    numMsg++;
    absoluteError += tempRttError;
    relativeError += tempRttError/SIMTIME_DBL(rtt);

    globalStatistics->recordOutVector("Vivaldi: absolute Rtt Error",
                                      tempRttError);
    globalStatistics->recordOutVector("Vivaldi: relative Rtt Error",
                                      (tempRttError/SIMTIME_DBL(rtt)));
}

bool NeighborCache::checkEntry(const TransportAddress &node)
{
    //countCheckEntryTotal++;
    if (neighborCache.count(node) == 0) {
        return false;

    }
    //countCheckEntrySuccessful++;
    //relativeCheckEntry = countCheckEntrySuccessful /
    //countCheckEntryTotal * 100;
    return true;
}

simtime_t NeighborCache::getMeanRtt(const TransportAddress &node)
{
    int size = 0;
    simtime_t rttSum = 0;
    if (neighborCache.count(node) == 0) {
        throw cRuntimeError("NeighborCache.cc: getMeanRtt was asked for"
                            "a non-existent node reference.");
    }

    size = neighborCache[node].lastRtts.size();

    neighborCache[node].coordsInfo;

    for (int i = 0; i < size; i++){
        rttSum += neighborCache[node].lastRtts.at(i);
    }
    return (rttSum / size);
}


double NeighborCache::getVarRtt(const TransportAddress &node, simtime_t &meanRtt)
{
    int size = 0;
    double sum = 0.0;
    simtime_t tempRtt;

    if (neighborCache.count(node) == 0) {
        throw cRuntimeError("NeighborCache.cc: getMeanRtt was asked for"
                            "a non-existent node reference.");
    }

    meanRtt = getMeanRtt(node);
    size = neighborCache[node].lastRtts.size();
    for (int i = 0; i < size; i++){
        tempRtt = neighborCache[node].lastRtts.at(i) - meanRtt;
        sum += (SIMTIME_DBL(tempRtt) * SIMTIME_DBL(tempRtt));
    }

    if (size < 2) {
        return 0;
    }

    if (size == 0) return -1;
    return (sum / (size -1));
}
