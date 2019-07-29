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
 * @file NeighborCache.h
 * @author Antonio Zea
 * @author Bernhard Heep
 */


#ifndef __NEIGHBORCACHE_H_
#define __NEIGHBORCACHE_H_

#include <omnetpp.h>

#include <map>
#include <cfloat>

#include <BaseApp.h>
#include <NodeHandle.h>
#include <Nps.h>
#include <CoordinateSystem.h>
#include <ProxNodeHandle.h>
#include <HashFunc.h>

class Vivaldi;
class GlobalStatistics;
class TransportAddress;
class RpcListener;


// Prox stuff
enum NeighborCacheQueryType {
    NEIGHBORCACHE_AVAILABLE,     //< RTT, timeout, or unknown (no query)
    NEIGHBORCACHE_EXACT,         //< RTT or query
    NEIGHBORCACHE_EXACT_TIMEOUT, //< RTT, timeout, or query
    NEIGHBORCACHE_ESTIMATED,     //< RTT or estimated
    NEIGHBORCACHE_QUERY,         //< only query, return unknown
    // default
    NEIGHBORCACHE_DEFAULT,       //< available, exact, exact_timeout or estimated
    NEIGHBORCACHE_DEFAULT_IMMEDIATELY, //< return a result immediately (available or estimated)
    NEIGHBORCACHE_DEFAULT_QUERY  //< do a query if needed (exact, exact_timeout, or query)
};

class ProxListener {
public:
    virtual void proxCallback(const TransportAddress& node, int rpcId,
                              cPolymorphic *contextPointer, Prox prox) = 0;
};

class NeighborCache : public BaseApp
{
    friend class Nps;

private:
    // parameters
    bool enableNeighborCache;
    simtime_t rttExpirationTime;
    uint32_t maxSize;

    bool enableNps;
    bool enableVivaldi;

    uint32_t misses;
    uint32_t hits;

    bool cleanupCache();

    void updateEntry(const TransportAddress& address,
                     simtime_t insertTime);

    Vivaldi* vivaldi;
    Nps* nps;

    NeighborCacheQueryType defaultQueryType;
    NeighborCacheQueryType defaultQueryTypeI;
    NeighborCacheQueryType defaultQueryTypeQ;

    Prox getCoordinateBasedProx(const TransportAddress& node);

    cMessage *landmarkTimer;

    static const std::vector<double> coordsDummy;

    // TODO roth
    //------
    void calcRttError(const NodeHandle &handle, simtime_t rtt);
    uint32_t numMsg;
    double absoluteError;
    double relativeError;
    uint32_t numRttErrorToHigh;
    uint32_t numRttErrorToLow;
    uint32_t rttHistory;
    uint32_t rttErrorHistory;

    /*
    double countGetProxTotal;
    double countGetProxSuccessful;
    double relativeGetProx;

    double countCheckEntryTotal;
    double countCheckEntrySuccessful;
    double relativeCheckEntry;
    */
    std::map<TransportAddress, std::vector<double> > lastAbsoluteErrorPerNode;
    //-------

    struct WaitingContexts
    {
        std::vector<ProxListener*> pingListeners;
        std::vector<cPolymorphic*> pingContexts;
        std::vector<uint32_t> pingIds;

        inline uint32_t size() { return pingContexts.size(); };
    };

    // ping context stuff
    bool insertNodeContext(const TransportAddress& handle,
                           cPolymorphic* context,
                           ProxListener* rpcListener,
                           int rpcId);

    NeighborCache::WaitingContexts* getNodeContexts(const TransportAddress& handle);

    enum NeighborCacheRttState {
        RTTSTATE_VALID,
        RTTSTATE_UNKNOWN,
        RTTSTATE_TIMEOUT,
        RTTSTATE_WAITING
    };

    typedef std::pair<simtime_t, NeighborCacheRttState> Rtt;

    Rtt getNodeRtt(const TransportAddress& add);

protected:
    GlobalStatistics* globalStatistics;

    struct ProxContext {
        int rpcId;
        ProxListener *listener;
        cPolymorphic *contextPointer;
    };



    struct NeighborCacheEntry {
        NeighborCacheEntry() { waitingContexts = NULL;
                               proxContext = NULL;
                               rttState = RTTSTATE_UNKNOWN; };

        ~NeighborCacheEntry() { };

        simtime_t  insertTime;
        simtime_t  rtt;
        NeighborCacheRttState rttState;
        //TODO roth
        //------
        std::vector<simtime_t> lastRtts;
        uint32_t rttCounter;
        //------
        NodeHandle nodeRef;
        NodeHandle srcRoute;
        NodeCoordsInfo coordsInfo;

        ProxContext* proxContext; /**< context of first query (getProx) */
        WaitingContexts* waitingContexts; //TODO ptr->object
    };

    UNORDERED_MAP<TransportAddress, NeighborCacheEntry> neighborCache;
    typedef UNORDERED_MAP<TransportAddress, NeighborCacheEntry>::iterator NeighborCacheIterator;
    typedef UNORDERED_MAP<TransportAddress, NeighborCacheEntry>::const_iterator NeighborCacheConstIterator;

    std::multimap<simtime_t, TransportAddress> neighborCacheExpireMap;
    typedef std::multimap<simtime_t, TransportAddress>::iterator neighborCacheExpireMapIterator;

    void initializeApp(int stage);

    void finishApp();

    virtual CompType getThisCompType() { return NEIGHBORCACHE_COMP; };

    void handleTimerEvent(cMessage* msg);

    /** Sends a pingNode call based on parameters from a getProx call.
        @param node The node to which the pingNode call will be sent.
        @param rpcId The rpcId that was passed to getProx.
        @param listener The listener that was passed to getProx.
        @param contextPointer The pointer that was passed to getProx.
    */
    void queryProx(const TransportAddress &node,
                   int rpcId,
                   ProxListener *listener,
                   cPolymorphic *contextPointer);

    /**
     *  Coord / RTT measuring rpc stuff goes here
     */
    bool handleRpcCall(BaseCallMessage* msg);

public:
    ~NeighborCache();


    inline bool isEnabled() { return enableNeighborCache; };

    inline bool npsEnabled() { return enableNps; };
    Nps& getNpsAccess()
    {
        if (nps == NULL)
            throw cRuntimeError("NPS module uninitialized");
        else
            return *nps;
    };

    inline bool vivaldiEnabled() { return enableVivaldi; };
    const Vivaldi& getVivaldiAccess()
    {
        if (vivaldi == NULL)
            throw cRuntimeError("Vivaldi module uninitialized");
        else
            return *vivaldi;
    };

    uint16_t getNeighborCacheSize() { return neighborCache.size(); };

    // getter for specific node information
    bool isEntry(const TransportAddress& node);
    simtime_t getNodeAge(const TransportAddress& handle);
    const NodeHandle& getNodeHandle(const TransportAddress &add);

    // getter for general node information
    TransportAddress getNearestNode(uint8_t maxLayer);
    double getAvgAbsPredictionError(std::vector<double>& coord,
                                    uint32_t sampleSize);

    // setter for specific node information
    void updateNode(const NodeHandle &add, simtime_t rtt,
                    const NodeHandle& srcRoute = NodeHandle::UNSPECIFIED_NODE,
                    const std::vector<double>& coords = coordsDummy,
                    double error = 1.0);
    void setNodeTimeout(const TransportAddress& handle);
    void setNodeLayer(const NodeHandle& add, int8_t layer);

    /***** NPS stuff *****/ //TODO
    //int8_t getAddressLayer(const IPvXAddress& add);
    std::vector<LandmarkDataEntry> getLandmarkData(const std::vector<TransportAddress>& landmarkSet) const;
    bool setLandmarkSet(uint8_t dim, uint8_t maxLayer, std::vector<TransportAddress>* landmarkSet);
    double getOwnEuclidianDistanceToKey(const OverlayKey& destKey) const; //TODO remove

    /** Gets the proximity of a node.
    @param node The node whose proximity will be requested.
    @param type Request type.
                NEIGHBORCACHE_EXACT looks in the cache, and if no value
                is found, sends an RTT query to the node.
                NEIGHBORCACHE_AVAILABLE looks in the cache, and if no value
                is found returns Prox::PROX_UNKNOWN.
                NEIGHBORCACHE_ESTIMATED looks in the cache, and if no value
                is found calculates an estimate based on information collected
                by the overlay.
                NEIGHBORCACHE_QUERY always sends an RTT query to the node and
                returns Prox::PROX_UNKNOWN.
    @param rpcId Identifier sent to the RPC to identify the call.
    @param listener Module to be called back when an RTT response arrives.
    @param contextPointer Pointer sent to the RPC to identify the call.
           IMPORTANT: contextPointer gets deleted (only) if no Ping call is sent!
           So, *contextpointer may be undefined!
    @returns The proximity value of node.
    */
    Prox getProx(const TransportAddress &node,
                 NeighborCacheQueryType type = NEIGHBORCACHE_AVAILABLE,
                 int rpcId = -1,
                 ProxListener *listener = NULL,
                 cPolymorphic *contextPointer = NULL);

    /** Estimates a Prox value of node, in relation to this node, based on information
        collected by the overlay.
        @param node The node whose proximity will be requested.
        @returns The RTT value if one is found in the cache, or else a Prox estimate.
    */
    Prox estimateProx(const TransportAddress &node);

    /** Returns the coordinate information of a node.
        @param node The node whose coordinate information will be requested.
        @returns The coordinate information.
    */
    const NodeCoordsInfo& getNodeCoordsInfo(const TransportAddress &node);

    /** Sets the coordinate information of a node.
        @param node The node whose coordinate information will be set.
        @param coords The coordinate information.
    */
    void setNodeCoordsInfo(const TransportAddress &node, const NodeCoordsInfo &coords);

    //TODO roth
    //------
    bool checkEntry(const TransportAddress &node);
    simtime_t getMeanRtt(const TransportAddress &node);
    double getVarRtt(const TransportAddress &node, simtime_t &meanRtt);
    //------

    friend std::ostream& operator<<(std::ostream& os,
                                    const NeighborCacheEntry& entry);
};

#endif
