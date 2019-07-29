//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef MDHT_H_
#define MDHT_H_

#include <omnetpp.h>
#include <MDHTBaseApp.h>
#include <CommonMessages_m.h>
#include <HostnameInfo.h>
#include <HashFunc.h>

#include "mDHT_m.h"

class mDHT : public MDHTBaseApp {
private:

    /**
     * A container used by the DHTTestApp to
     * store context information for statistics
     *
     * @author Ingmar Baumgart
     */
    class DHTStatsContext : public cPolymorphic
    {
    public:
        bool measurementPhase;
        OverlayKey key;

        DHTStatsContext(bool measurementPhase,
                        const OverlayKey& key) :
            measurementPhase(measurementPhase),
            key(key) {};
    };


	void initializeApp(int stage);

	void finishApp();


	int numGetRequests, numPutRequests, numAddRequests, numRemoveRequests;
        int numSuccGet, numSuccPut, numSuccAdd, numSuccRemove;
        int numFailGet, numFailPut, numFailAdd, numFailRemove;
        int numInternalGetRequests, numCopyHits, numHits;
        int numRequests;

protected:

	//Send RPC Calls
	bool handleRpcCall(BaseCallMessage* msg);
    void handleTimerEvent(cMessage* msg);
    void handlePutAPIRequest(NetInfAPIPutCall* apiPutMsg);
    void handleGetAPIRequest(NetInfAPIGetCall* apiGetMsg);
    void handleAddLocatorAPIRequest(NetInfAPIAddLocatorCall* apiAddMsg);
    void handleRemoveLocatorAPIRequest(NetInfAPIRemoveLocatorCall* apiRemoveMsg);
    void handlePutRequest(mDHTPutCall* apiPutMsg);
    void handleGetRequest(mDHTGetCall* apiPutMsg);
    void handleAddLocatorRequest(mDHTAddLocatorCall* apiAddMsg);
    void handleRemoveLocatorRequest(mDHTRemoveLocatorCall* apiRemoveMsg);
    void handleFastDataRequest(mDHTFastDataCall* fastDataMsg);
    //RPC Response handler
    virtual void handlePutResponse(mDHTPutResponse* msg,
                                   DHTStatsContext* context,
                                   simtime_t rtt,
                                   int rpcId);
    virtual void handleGetResponse(mDHTGetResponse* msg,
                                   DHTStatsContext* context,
                                   simtime_t rtt,
                                   int rpcId);
    virtual void handleAddLocatorResponse(mDHTAddLocatorResponse* msg,
                                   DHTStatsContext* context,
                                   simtime_t rtt,
                                   int rpcId);
    virtual void handleRemoveLocatorResponse(mDHTRemoveLocatorResponse* msg,
                                   DHTStatsContext* context,
                                   simtime_t rtt,
                                   int rpcId);
    void handleInternalGetResponse(int rpcId, const OverlayKey& key, int ttl, bool success, const std::string& ownerString, const std::string& locatorString, int version, const std::list<TransportAddress>& path);
    void handleInternalPutResponse(int rpcId, const OverlayKey& key, bool success, const std::list<TransportAddress>& path);
    void handleInternalAddLocatorResponse(int rpcId, const OverlayKey& key, bool success, const std::list<TransportAddress>& path);
    void handleInternalRemoveLocatorResponse(int rpcId, const OverlayKey& key, bool success, const std::list<TransportAddress>& path); 
    void handleInternalGetRequest(uint32_t rpcId);
    void handleInternalPutRequest(uint32_t rpcId);
    void handleInternalAddLocatorRequest(uint32_t rpcId);
    void handleInternalRemoveLocatorRequest(uint32_t rpcId);
    void handleInternalFastDataRequest(int rpcId, const OverlayKey& key, const std::string& ownerString, const std::string& locatorString, int version, int ttl, bool success);

    void handleRpcResponse(BaseResponseMessage* msg,
                               cPolymorphic* context,
                               int rpcId,
                               simtime_t rtt);
    void handleRpcTimeout(BaseCallMessage* msg,
                                 const TransportAddress& dest,
                                 cPolymorphic* context, int rpcId,
                                 const OverlayKey&);

    int numLevels;
    int cacheTtl;
    int maxCacheSize;
    int copyRefreshTime;
    bool passiveCache;
    std::vector<double> perLevelDelayFactor;
    int statisticsSampleInterval;
    bool enableTimeStatistics;

    struct GetMapEntry
    {
        NetInfAPIGetCall* acallMsg;
        mDHTGetCall * icallMsg;

        std::string originator;

        GetMapEntry() : acallMsg(0), icallMsg(0), originator() {}
    };

    struct PutMapEntry
    {
        NetInfAPIPutCall* acallMsg;
        mDHTPutCall* icallMsg;

        std::string ownerString;
        PutMapEntry() : acallMsg(0), icallMsg(0), ownerString() {}
    };

    struct AddMapEntry
    {
        NetInfAPIAddLocatorCall* acallMsg;
        mDHTAddLocatorCall* icallMsg;

        std::string locatorString;
        AddMapEntry() : acallMsg(0), icallMsg(0), locatorString() {}
    };

    struct RemoveMapEntry
    {
        NetInfAPIRemoveLocatorCall* acallMsg;
        mDHTRemoveLocatorCall* icallMsg;

        std::string locatorString;
        RemoveMapEntry() : acallMsg(0), icallMsg(0), locatorString() {}
    };

    // original nonce->entry
    UNORDERED_MAP<int, GetMapEntry> getMap;
    // original nonce->entry
    UNORDERED_MAP<int, PutMapEntry> putMap;
    // original nonce->entry
    UNORDERED_MAP<int, AddMapEntry> addMap;
    // original nonce->entry
    UNORDERED_MAP<int, RemoveMapEntry> removeMap;

    typedef UNORDERED_MAP<uint32_t, BaseCallMessage*> DelayedCalls;
    DelayedCalls delayedCalls;

    struct DataStorageEntry
    {
      OverlayKey key;
      
      std::string ownerString;

      int version;
      
      // (locator, version)
      std::map<std::string, std::pair<int, bool> > secondaryLocators;

      bool isCopy;
      bool isCache;

      cMessage *ttlMessage;

      DataStorageEntry(const OverlayKey& key, const std::string& ownerString, int version, bool isCopy, bool isCache, cMessage *ttlMessage) :
          key(key), ownerString(ownerString), version(version), isCopy(isCopy), isCache(isCache), ttlMessage(ttlMessage) {}
    };

    class DataStorage {
    private:
      typedef UNORDERED_MAP<OverlayKey, DataStorageEntry> EntryMap;
      EntryMap entries;
      std::list<OverlayKey> lru;
      int maxCacheSize;
      cSimpleModule* module;
    public:
      DataStorage(cSimpleModule* module);
      DataStorageEntry* get(const OverlayKey& key);
      void add(const OverlayKey& key, const std::string& ownerString, int version, bool isCopy, bool isCache, int ttl);
      void remove(const OverlayKey& key);


      void addSecondaryLocator(const OverlayKey& key, const std::string& locatorString, int version, bool isCache);
      void removeSecondaryLocator(const OverlayKey& key, const std::string& locatorString);

      void clear();
      void setMaxCacheSize(int size);

      int size() const { return entries.size(); }
      int copies() const
      {
        EntryMap::const_iterator iter;
        int copies = 0;
        for (iter = entries.begin(); iter != entries.end(); iter++)
          if (iter->second.isCopy)
            copies++;
        return copies;
      }
    };
    DataStorage dataStorage;

    HostnameInfo* hostnameInfo;
    std::string stringName;

    cMessage *statisticsTimer;
    int numSampledGets, numSampledHits, numSampledCopyHits;
    int numSampledRequests;

public:
	mDHT();
	virtual ~mDHT();
};

#endif /* MDHT_H_ */
