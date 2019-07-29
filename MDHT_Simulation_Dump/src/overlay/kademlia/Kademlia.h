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
 * @file Kademlia.h
 * @author Sebastian Mies, Ingmar Baumgart, Bernhard Heep
 */

#ifndef __KADEMLIA_H_
#define __KADEMLIA_H_

#include <omnetpp.h>

#include <CommonMessages_m.h>
#include <BaseOverlay.h>
#include <GlobalStatistics.h>
#include <NeighborCache.h>

#include "KademliaNodeHandle.h"
#include "KademliaBucket.h"

/**
 * Kademlia overlay module
 *
 * @author Sebastian Mies, Ingmar Baumgart, Bernhard Heep
 */
class Kademlia : public BaseOverlay
{
protected://fields: kademlia parameters

    uint32_t k; /*< number of redundant graphs */
    uint32_t b; /*< number of bits to consider */
    uint32_t s; /*< number of siblings         */

    uint32_t maxStaleCount; /*< number of timouts until node is removed from
                            routingtable */

    bool exhaustiveRefresh; /*< if true, use exhaustive-iterative lookups to refresh buckets */
    bool pingNewSiblings;

    simtime_t minSiblingTableRefreshInterval;
    simtime_t minBucketRefreshInterval;

    cMessage* bucketRefreshTimer;

public:
    Kademlia();

    ~Kademlia();

    void initializeOverlay(int stage);

    void finishOverlay();

    void joinOverlay();

    bool isSiblingFor(const NodeHandle& node,const OverlayKey& key,
                      int numSiblings, bool* err );

    int getMaxNumSiblings();

    int getMaxNumRedundantNodes();

    void handleTimerEvent(cMessage* msg);

    bool handleRpcCall(BaseCallMessage* msg);

protected:
    NodeVector* findNode(const OverlayKey& key,
                         int numRedundantNodes,
                         int numSiblings,
                         BaseOverlayMessage* msg);

    void handleRpcResponse(BaseResponseMessage* msg,
                           cPolymorphic* context,
                           int rpcId, simtime_t rtt);

    void handleRpcTimeout(BaseCallMessage* msg, const TransportAddress& dest,
                          cPolymorphic* context, int rpcId,
                          const OverlayKey& destKey);

    /**
     * handle a expired bucket refresh timer
     */
    void handleBucketRefreshTimerExpired();

    OverlayKey distance(const OverlayKey& x,
                        const OverlayKey& y) const;

    /**
     * updates information shown in GUI
     */
    void updateTooltip();

    virtual void lookupFinished(bool isValid);

    friend class KademliaLookupListener;


private:
    uint32_t bucketRefreshCount; /*< statistics: total number of bucket refreshes */
    uint32_t siblingTableRefreshCount; /*< statistics: total number of sibling table refreshes */
    uint32_t nodesReplaced;

    KeyDistanceComparator<KeyXorMetric>* comparator;

    KademliaBucket*  siblingTable;
    std::vector<KademliaBucket*> routingTable;
    int numBuckets;

    std::map<NodeHandle, NodeHandle> replacementCache;
    void routingInit();

    void routingDeinit();

    /**
     * Returns the index of the bucket the key would reside
     * with respect to Kademlia parameters
     *
     * @param key The key of the node
     * @param firstOnLayer If true bucket with smallest index on same layer
     *                     is returned
     * @return int The index of the bucket
     */
    int routingBucketIndex(const OverlayKey& key, bool firstOnLayer = false);

    /**
     * Returns a Bucket or <code>NULL</code> if the bucket has not
     * yet allocated. If ensure is true, the bucket allocation is
     * ensured.
     *
     * @param key The key of the node
     * @param ensure If true, the bucket allocation is ensured
     *
     * @return Bucket* The Bucket
     */
    KademliaBucket* routingBucket(const OverlayKey& key, bool ensure);

    /**
     * Adds a node to the routing table
     *
     * @param handle handle to add
     * @param isAlive true, if it is known that the node is alive
     * @param rtt measured round-trip-time to node
     * @return true, if the node was known or has been added
     */
    bool routingAdd(const NodeHandle& handle, bool isAlive,
                    simtime_t rtt = MAXTIME);

    /**
     * Removes a node from the routing table
     *
     * @param key Key of the Node
     * @return true, if the node has been removed
     */
    bool routingRemove(const OverlayKey& key);

    /**
     * Removes a node after a number of timeouts or immediately
     * if immediately is true (behaves like routingRemove).
     *
     * @param key Node's key to remove
     * @param immediately If true, the node is removed immediately
     * @return true, if the node has been removed
     */
    bool routingTimeout(const OverlayKey& key, bool immediately = false);

    void refillSiblingTable();

    void setBucketUsage(const OverlayKey& key);

    bool handleFailedNode(const TransportAddress& failed);
};

#endif
