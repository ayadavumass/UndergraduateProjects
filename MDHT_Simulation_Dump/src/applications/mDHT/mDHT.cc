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

#include "mDHT.h"

#include <GlobalStatisticsAccess.h>
#include <MultipleDHTAccess.h>

#include "mDHT_m.h"

using namespace std;

Define_Module(mDHT);

mDHT::mDHT() : dataStorage(this) {
	// Auto-generated constructor stub

}

mDHT::~mDHT() {
	// Auto-generated destructor stub
}

void mDHT::initializeApp(int stage) {
    if (stage != MIN_STAGE_APP)
        return;

    globalStatistics = GlobalStatisticsAccess().get();
    hostnameInfo = dynamic_cast<HostnameInfo*>(simulation.getModuleByPath(
              "globalObserver.globalFunctions[1].function"));
    assert(hostnameInfo != NULL);

    thisNode.setPort(2000);
    stringName = hostnameInfo->getHostname(thisNode.getAddress());

    cacheTtl = par("cacheTtl");
    copyRefreshTime = par("copyRefreshTime");
    maxCacheSize = par("maxCacheSize");
    dataStorage.setMaxCacheSize(maxCacheSize);
    passiveCache = par("passiveCache") && (maxCacheSize != 0);
    numLevels = par("numLevels");
    {
      std::string factors = par("perLevelDelayFactor");
      const char *s = factors.c_str();
      char *endptr = NULL;

      do {
        double d = strtod(s, &endptr);
        perLevelDelayFactor.push_back(d);
        if (endptr == NULL || *endptr == '\0')
          break;
        else
          s = endptr + 1;
      } while (true);
      assert(perLevelDelayFactor.size() == numLevels-1);
    }
    statisticsSampleInterval = par("statisticsSampleInterval");
    enableTimeStatistics = par("enableTimeStatistics");

    if (enableTimeStatistics) {
      statisticsTimer = new cMessage("statistics_timer");
      scheduleAt(simTime() + statisticsSampleInterval, statisticsTimer);
    } else {
      statisticsTimer = NULL;
    }

    numRequests = 0;
    numGetRequests = 0;
    numPutRequests = 0;
    numAddRequests = 0;
    numRemoveRequests = 0;
    numSuccGet = 0;
    numSuccPut = 0;
    numSuccAdd = 0;
    numSuccRemove = 0;
    numFailGet = 0;
    numFailPut = 0;
    numFailAdd = 0;
    numFailRemove = 0;
    numInternalGetRequests = 0;
    numCopyHits = 0;
    numHits = 0;
    numSampledRequests = 0;
    numSampledCopyHits = 0;
    numSampledHits = 0;
    numSampledGets = 0;
    WATCH(numRequests);
    WATCH(numGetRequests);
    WATCH(numPutRequests);
    WATCH(numAddRequests);
    WATCH(numRemoveRequests);
    WATCH(numSuccGet);
    WATCH(numSuccPut);
    WATCH(numSuccAdd);
    WATCH(numSuccRemove);
    WATCH(numFailGet);
    WATCH(numFailPut);
    WATCH(numFailAdd);
    WATCH(numFailRemove);
    WATCH(numInternalGetRequests);
    WATCH(numCopyHits);
    WATCH(numHits);
}

void mDHT::handleTimerEvent(cMessage* msg)
{
  mDHTTtlMessage *ttlmessage = dynamic_cast<mDHTTtlMessage*> (msg);

  if (ttlmessage) {
    DataStorageEntry *entry = dataStorage.get(ttlmessage->getKey());

    if (entry) {
      if (entry->isCache) {
        entry->ttlMessage = NULL;
        dataStorage.remove(entry->key);
      } else if (entry->isCopy) {
        entry->ttlMessage = NULL;

        mDHTGetCall *gcall = new mDHTGetCall();
        gcall->setKey(entry->key);
        gcall->setLevel(0);
        gcall->setOriginator(stringName.c_str());
        
        sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, 0,
            TransportAddress::UNSPECIFIED_NODE,
            entry->key, gcall,
              new DHTStatsContext(globalStatistics->isMeasuring(), entry->key),
              DEFAULT_ROUTING, -1, 0, -1, NULL);
      } else {
        assert(false);
      }
    }
  }

  mDHTDelayMessage *dmsg = dynamic_cast<mDHTDelayMessage*> (msg);
  if (dmsg) {
    std::list<TransportAddress> path;
    int n = dmsg->getPathArraySize(), i;
    for (i = 0; i < n; i++)
      path.push_back(dmsg->getPath(i));

    int delayedType = dmsg->getDelayedType();
    bool req = dmsg->getRequest();
    if (delayedType == 0) {
      if (req)
        handleInternalPutRequest(dmsg->getRpcId());
      else
        handleInternalPutResponse(dmsg->getRpcId(), dmsg->getKey(), dmsg->getSuccess(), path);
    } else if (delayedType == 1) {
      if (req)
        handleInternalGetRequest(dmsg->getRpcId());
      else
        handleInternalGetResponse(dmsg->getRpcId(), dmsg->getKey(), dmsg->getTtl(), dmsg->getSuccess(), dmsg->getOwnerString(), dmsg->getLocatorString(), dmsg->getVersion(), path);
    } else if (delayedType == 2) {
      if (req)
        handleInternalAddLocatorRequest(dmsg->getRpcId());
      else
        handleInternalAddLocatorResponse(dmsg->getRpcId(), dmsg->getKey(), dmsg->getSuccess(), path);
    } else if (delayedType == 3) {
      if (req)
        handleInternalRemoveLocatorRequest(dmsg->getRpcId());
      else
        handleInternalRemoveLocatorResponse(dmsg->getRpcId(), dmsg->getKey(), dmsg->getSuccess(), path);
    } else if (delayedType == 4) {
      handleInternalFastDataRequest(dmsg->getRpcId(), dmsg->getKey(), dmsg->getOwnerString(), dmsg->getLocatorString(), dmsg->getVersion(), dmsg->getTtl(), dmsg->getSuccess());
    }
  }

  if (msg == statisticsTimer) {
    scheduleAt(simTime() + statisticsSampleInterval, statisticsTimer);
    
    if (enableTimeStatistics) {
      if (numSampledGets > 0) {
        globalStatistics->recordOutVector("mDHT: Hit ratio", numSampledHits / ((double) numSampledGets));
        globalStatistics->recordOutVector("mDHT: Copy hit ratio", numSampledCopyHits / ((double) numSampledGets));
        numSampledGets = numSampledHits = numSampledCopyHits = 0;
      }
      globalStatistics->recordOutVector("mDHT: Requests/s", numSampledRequests / ((double) statisticsSampleInterval));
      numSampledRequests = 0;

      globalStatistics->recordOutVector("mDHT: Stored entries", dataStorage.size());
      globalStatistics->recordOutVector("mDHT: Stored copies", dataStorage.copies());
    }
  } else {
    cancelAndDelete(msg);
  }
}

bool mDHT::handleRpcCall(BaseCallMessage* msg) {
    RECORD_STATS(numRequests++; numSampledRequests++);

    // delegate messages
    RPC_SWITCH_START( msg )
        RPC_DELEGATE( NetInfAPIPut, handlePutAPIRequest );
        RPC_DELEGATE( NetInfAPIGet, handleGetAPIRequest );
        RPC_DELEGATE( NetInfAPIAddLocator, handleAddLocatorAPIRequest );
        RPC_DELEGATE( NetInfAPIRemoveLocator, handleRemoveLocatorAPIRequest );
        RPC_DELEGATE( mDHTGet, handleGetRequest );
        RPC_DELEGATE( mDHTPut, handlePutRequest );
        RPC_DELEGATE( mDHTAddLocator, handleAddLocatorRequest );
        RPC_DELEGATE( mDHTRemoveLocator, handleRemoveLocatorRequest );
        RPC_DELEGATE( mDHTFastData, handleFastDataRequest );
    RPC_SWITCH_END( )

    return RPC_HANDLED;
}

void mDHT::handlePutAPIRequest(NetInfAPIPutCall* pcall) {
	EV << "mDHT: PUT API CALL arrived\n";
    PutMapEntry putEntry;

    RECORD_STATS(numPutRequests++);
    
    putEntry.acallMsg = pcall;

    {
      std::ostringstream os;
      os << stringName << ":" << pcall->getOwnerUser();
      putEntry.ownerString = os.str();
     }
    
    putMap.insert(make_pair (pcall->getNonce(), putEntry));

    mDHTPutCall* pcall2 = new mDHTPutCall();
    pcall2->setKey(pcall->getKey());
    pcall2->setOwnerString(putEntry.ownerString.c_str());
    pcall2->setVersion(pcall->getVersion());
    pcall2->setLevel(0);

    sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, 0, 
         TransportAddress::UNSPECIFIED_NODE,
         pcall->getKey(), pcall2,
         new DHTStatsContext(globalStatistics->isMeasuring(),
              pcall->getKey()),
          DEFAULT_ROUTING, -1, 0, pcall->getNonce(), NULL);
}

void mDHT::handleInternalPutRequest(uint32_t rpcId)
{
	EV << "mDHT: PUT CALL arrived\n";

    DelayedCalls::iterator iter = delayedCalls.find(rpcId);
    if (iter == delayedCalls.end())
      return;

    mDHTPutCall *pcall = static_cast<mDHTPutCall*>(iter->second);
    delayedCalls.erase(iter);

    DataStorageEntry* entry = dataStorage.get(pcall->getKey());
    if (strlen(pcall->getOwnerString()) == 0 || (entry && entry->ownerString != pcall->getOwnerString())) {
      mDHTPutResponse *presp = new mDHTPutResponse();
      presp->setKey(pcall->getKey());
      presp->setSuccess(false);
      presp->setLevel(pcall->getLevel());
      presp->setPathArraySize(1);
      presp->setPath(0, thisNode);

      sendRpcResponse(pcall, presp);
      return;
    }
    
    dataStorage.add(pcall->getKey(), pcall->getOwnerString(), pcall->getVersion(), false, false, 0);
    if (pcall->getLevel() < numLevels - 1) {
      PutMapEntry putEntry;
      putEntry.icallMsg = pcall;
      putEntry.ownerString = pcall->getOwnerString();
      putMap.insert(make_pair (pcall->getNonce(), putEntry));

      mDHTPutCall *pcall2 = new mDHTPutCall();
      pcall2->setKey(pcall->getKey());
      pcall2->setOwnerString(pcall->getOwnerString());
      pcall2->setLevel(pcall->getLevel()+1);
      
      sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, pcall2->getLevel(), 
         TransportAddress::UNSPECIFIED_NODE,
         pcall->getKey(), pcall2,
         new DHTStatsContext(globalStatistics->isMeasuring(),
              pcall->getKey()),
          DEFAULT_ROUTING, -1, 0, pcall->getNonce(), NULL);
    } else {
      mDHTPutResponse *presp = new mDHTPutResponse();
      presp->setKey(pcall->getKey());
      presp->setSuccess(true);
      presp->setLevel(pcall->getLevel());

      presp->setPathArraySize(1);
      presp->setPath(0, thisNode);

      sendRpcResponse(pcall, presp);
    }
}

void mDHT::handlePutRequest(mDHTPutCall* pcall)
{
  delayedCalls.insert(std::make_pair(pcall->getNonce(), pcall));

  if (pcall->getCreationTime() != simTime() && pcall->getLevel() > 0 && perLevelDelayFactor[pcall->getLevel()-1] > 1.0) {
    mDHTDelayMessage *dmsg = new mDHTDelayMessage();

    dmsg->setDelayedType(0);
    dmsg->setRpcId(pcall->getNonce());
    dmsg->setRequest(true);

    scheduleAt(simTime() + (perLevelDelayFactor[pcall->getLevel()-1] - 1.0) * (simTime() - pcall->getCreationTime()), dmsg);
  } else {
    handleInternalPutRequest(pcall->getNonce());
  }
}

void mDHT::handleAddLocatorAPIRequest(NetInfAPIAddLocatorCall* acall) {
	EV << "mDHT: ADDLOCATOR API CALL arrived\n";

    RECORD_STATS(numAddRequests++);

    AddMapEntry addEntry;
    
    addEntry.acallMsg = acall;

     {
      std::ostringstream os;
      os << stringName << ":" << acall->getLocatorUser();
      addEntry.locatorString = os.str();
     }
    
    addMap.insert(make_pair (acall->getNonce(), addEntry));

    mDHTAddLocatorCall* acall2 = new mDHTAddLocatorCall();
    acall2->setKey(acall->getKey());
    acall2->setOwnerString(acall->getOwnerString());
    acall2->setLocatorString(addEntry.locatorString.c_str());
    acall2->setVersion(acall->getVersion());
    acall2->setLevel(0);

    sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, 0,
        TransportAddress::UNSPECIFIED_NODE,
        acall->getKey(), acall2,
        new DHTStatsContext(globalStatistics->isMeasuring(),
             acall->getKey()),
          DEFAULT_ROUTING, -1, 0, acall->getNonce(), NULL);
}

void mDHT::handleInternalAddLocatorRequest(uint32_t rpcId) {
	EV << "mDHT: ADDLOCATOR CALL arrived\n";

    DelayedCalls::iterator iter = delayedCalls.find(rpcId);
    if (iter == delayedCalls.end())
      return;

    mDHTAddLocatorCall *acall = static_cast<mDHTAddLocatorCall*>(iter->second);
    delayedCalls.erase(iter);

    DataStorageEntry* entry = dataStorage.get(acall->getKey());
    if (entry && entry->ownerString != acall->getOwnerString()) {
      mDHTAddLocatorResponse *aresp = new mDHTAddLocatorResponse();

      aresp->setKey(acall->getKey());
      aresp->setSuccess(false);
      aresp->setLevel(acall->getLevel());
      aresp->setPathArraySize(1);
      aresp->setPath(0, thisNode);

      sendRpcResponse(acall, aresp);
      return;
    } else if (!entry || entry->version < acall->getVersion()) {
      dataStorage.add(acall->getKey(), acall->getOwnerString(), acall->getVersion(), true, false, copyRefreshTime);
      entry = dataStorage.get(acall->getKey());
    }

    dataStorage.addSecondaryLocator(acall->getKey(), acall->getLocatorString(), acall->getVersion(), false);

    if (acall->getLevel() < numLevels - 1) {
      AddMapEntry addEntry;
      addEntry.icallMsg = acall;
      addEntry.locatorString = acall->getLocatorString();
      addMap.insert(make_pair (acall->getNonce(), addEntry));

      mDHTAddLocatorCall *acall2 = new mDHTAddLocatorCall();
      acall2->setKey(acall->getKey());
      acall2->setOwnerString(acall->getOwnerString());
      acall2->setLocatorString(acall->getLocatorString());
      acall2->setVersion(acall->getVersion());
      acall2->setLevel(acall->getLevel()+1);
      
      sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, acall2->getLevel(), 
         TransportAddress::UNSPECIFIED_NODE,
         acall->getKey(), acall2,
         new DHTStatsContext(globalStatistics->isMeasuring(),
              acall->getKey()),
          DEFAULT_ROUTING, -1, 0, acall->getNonce(), NULL);
    } else {
      mDHTAddLocatorResponse *aresp = new mDHTAddLocatorResponse();
      aresp->setKey(acall->getKey());
      aresp->setSuccess(true);
      aresp->setLevel(acall->getLevel());
      aresp->setPathArraySize(1);
      aresp->setPath(0, thisNode);
      
      sendRpcResponse(acall, aresp);
    }
}

void mDHT::handleAddLocatorRequest(mDHTAddLocatorCall* acall)
{
  delayedCalls.insert(std::make_pair(acall->getNonce(), acall));

  if (acall->getCreationTime() != simTime() && acall->getLevel() > 0 && perLevelDelayFactor[acall->getLevel()-1] > 1.0) {
    mDHTDelayMessage *dmsg = new mDHTDelayMessage();

    dmsg->setDelayedType(2);
    dmsg->setRpcId(acall->getNonce());
    dmsg->setRequest(true);

    scheduleAt(simTime() + (perLevelDelayFactor[acall->getLevel()-1] - 1.0) * (simTime() - acall->getCreationTime()), dmsg);
  } else {
    handleInternalAddLocatorRequest(acall->getNonce());
  }
}

void mDHT::handleRemoveLocatorAPIRequest(NetInfAPIRemoveLocatorCall* rcall) {
	EV << "mDHT: REMOVELOCATOR API CALL arrived\n";

    RECORD_STATS(numRemoveRequests++);

    RemoveMapEntry removeEntry;
    
    removeEntry.acallMsg = rcall;

    {
      std::ostringstream os;
      os << stringName << ":" << rcall->getLocatorUser();
      removeEntry.locatorString = os.str();
     }
    
    removeMap.insert(make_pair (rcall->getNonce(), removeEntry));

    mDHTRemoveLocatorCall* rcall2 = new mDHTRemoveLocatorCall();
    rcall2->setKey(rcall->getKey());
    rcall2->setLocatorString(removeEntry.locatorString.c_str());
    rcall2->setLevel(0);

    sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, 0,
        TransportAddress::UNSPECIFIED_NODE,
        rcall->getKey(), rcall2,
        new DHTStatsContext(globalStatistics->isMeasuring(),
             rcall->getKey()),
         DEFAULT_ROUTING, -1, 0, rcall->getNonce(), NULL);
}

void mDHT::handleInternalRemoveLocatorRequest(uint32_t rpcId) {
	EV << "mDHT: REMOVELOCATOR CALL arrived\n";

    DelayedCalls::iterator iter = delayedCalls.find(rpcId);
    if (iter == delayedCalls.end())
      return;

    mDHTRemoveLocatorCall *rcall = static_cast<mDHTRemoveLocatorCall*>(iter->second);
    delayedCalls.erase(iter);

    DataStorageEntry* entry = dataStorage.get(rcall->getKey());
    if (!entry) {
      mDHTRemoveLocatorResponse *rresp = new mDHTRemoveLocatorResponse();

      rresp->setKey(rcall->getKey());
      rresp->setSuccess(false);
      rresp->setLevel(rcall->getLevel());
      rresp->setPathArraySize(1);
      rresp->setPath(0, thisNode);

      sendRpcResponse(rcall, rresp);
      return;
    }

    dataStorage.removeSecondaryLocator(rcall->getKey(), rcall->getLocatorString());
    if (entry->isCopy && entry->secondaryLocators.empty())
      dataStorage.remove(entry->key);

    if (rcall->getLevel() < numLevels - 1) {
      RemoveMapEntry removeEntry;
      removeEntry.icallMsg = rcall;
      removeEntry.locatorString = rcall->getLocatorString();
      removeMap.insert(make_pair (rcall->getNonce(), removeEntry));

      mDHTRemoveLocatorCall *rcall2 = new mDHTRemoveLocatorCall();
      rcall2->setKey(rcall->getKey());
      rcall2->setLocatorString(rcall->getLocatorString());
      rcall2->setLevel(rcall->getLevel()+1);
      
      sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, rcall2->getLevel(), 
         TransportAddress::UNSPECIFIED_NODE,
         rcall->getKey(), rcall2,
         new DHTStatsContext(globalStatistics->isMeasuring(),
              rcall->getKey()),
          DEFAULT_ROUTING, -1, 0, rcall->getNonce(), NULL);
    } else {
      mDHTRemoveLocatorResponse *rresp = new mDHTRemoveLocatorResponse();
      rresp->setKey(rcall->getKey());
      rresp->setSuccess(true);
      rresp->setLevel(rcall->getLevel());
      rresp->setPathArraySize(1);
      rresp->setPath(0, thisNode);
      
      sendRpcResponse(rcall, rresp);
    }
}

void mDHT::handleRemoveLocatorRequest(mDHTRemoveLocatorCall* rcall) {
  delayedCalls.insert(std::make_pair(rcall->getNonce(), rcall));

  if (rcall->getCreationTime() != simTime() && rcall->getLevel() > 0 && perLevelDelayFactor[rcall->getLevel()-1] > 1.0) {
    mDHTDelayMessage *dmsg = new mDHTDelayMessage();

    dmsg->setDelayedType(3);
    dmsg->setRpcId(rcall->getNonce());
    dmsg->setRequest(true);

    scheduleAt(simTime() + (perLevelDelayFactor[rcall->getLevel()-1] - 1.0) * (simTime() - rcall->getCreationTime()), dmsg);
  } else {
    handleInternalRemoveLocatorRequest(rcall->getNonce());
  }
}

void mDHT::handleInternalFastDataRequest(int rpcId, const OverlayKey& key, const std::string& ownerString, const std::string& locatorString, int version, int ttl, bool success)
{
    UNORDERED_MAP<int, GetMapEntry>::iterator it3 = getMap.find(rpcId);
    if (it3 != getMap.end() && it3->second.acallMsg) {
      if ((it3->second.acallMsg || it3->second.icallMsg) && passiveCache && ttl > 0) {
        dataStorage.add(key, ownerString, version, true, true, ttl);
        if (locatorString != "") {
          dataStorage.addSecondaryLocator(key, locatorString, version, true);
        }
      }
      // Response erstellen
      NetInfAPIGetResponse* gresp = new NetInfAPIGetResponse();
      gresp->setKey(key);
      gresp->setTtl(ttl);
      gresp->setSuccess(success);
      gresp->setOwnerString(ownerString.c_str());
      gresp->setLocatorString(locatorString.c_str());
      gresp->setVersion(version);

      sendRpcResponse(it3->second.acallMsg, gresp);
      it3->second.acallMsg = NULL;
    }
}

void mDHT::handleFastDataRequest(mDHTFastDataCall* msg)
{
  if (msg->getCreationTime() != simTime() && msg->getLevel() > 0 && perLevelDelayFactor[msg->getLevel()-1] > 1.0) {
    mDHTDelayMessage *dmsg = new mDHTDelayMessage();

    dmsg->setDelayedType(4);
    dmsg->setRpcId(msg->getOriginatorRpcId());
    dmsg->setKey(msg->getKey());
    dmsg->setTtl(msg->getTtl());
    dmsg->setOwnerString(msg->getOwnerString());
    dmsg->setLocatorString(msg->getLocatorString());
    dmsg->setVersion(msg->getVersion());
    dmsg->setSuccess(msg->getSuccess());
    dmsg->setRequest(true);
          
     dmsg->setPathArraySize(0);

    scheduleAt(simTime() + (perLevelDelayFactor[msg->getLevel()-1] - 1.0) * (simTime() - msg->getCreationTime()), dmsg);
  } else {
    handleInternalFastDataRequest(msg->getOriginatorRpcId(), msg->getKey(), msg->getOwnerString(), msg->getLocatorString(), msg->getVersion(), msg->getTtl(), msg->getSuccess());
  }

  mDHTFastDataResponse* fresp = new mDHTFastDataResponse();
  sendRpcResponse(msg, fresp);
}

void mDHT::handleInternalGetRequest(uint32_t rpcId)
{
    DelayedCalls::iterator iter = delayedCalls.find(rpcId);
    if (iter == delayedCalls.end())
      return;

    mDHTGetCall *gcall = static_cast<mDHTGetCall*>(iter->second);
    delayedCalls.erase(iter);

    EV << "mDHT: GET CALL for key " << gcall->getKey() << " arrived \n";

    RECORD_STATS(numInternalGetRequests++; numSampledGets++);
    DataStorageEntry *entry = dataStorage.get(gcall->getKey());

    if (entry) {
      if (entry->isCopy)
        RECORD_STATS(numCopyHits++; numSampledCopyHits++);
      RECORD_STATS(numHits++; numSampledHits++);

      mDHTGetResponse *gresp = new mDHTGetResponse();
      gresp->setKey(entry->key);
      gresp->setTtl((entry->ttlMessage ? (int)SIMTIME_DBL(entry->ttlMessage->getArrivalTime() - simTime()) : cacheTtl));
      gresp->setSuccess(true);
      gresp->setOwnerString(entry->ownerString.c_str());
      gresp->setLevel(gcall->getLevel());
      gresp->setVersion(entry->version);

      gresp->setPathArraySize(1);
      gresp->setPath(0, thisNode);

      {
        int distance = -1;
        std::vector<std::string> bestLocators;
        std::map<std::string, std::pair<int, bool> >::const_iterator iter;
        std::string originator = gcall->getOriginator();

        for (iter = entry->secondaryLocators.begin(); iter != entry->secondaryLocators.end(); iter++) {
          if (iter->second.first < entry->version)
            continue;

          int n = std::min(iter->first.length(), originator.length());
          int d = 0;
          for (int i = 0; i < n; i++) {
            if (iter->first[i] != originator[i])
              break;
            if (iter->first[i] == '.' || iter->first[i] == ':')
              d++;
          }
          if (iter->first == originator) {
            bestLocators.clear();
            bestLocators.push_back(iter->first);      
            distance = d;
            break;
          } else if (d >= distance) {
            if (d > distance)
              bestLocators.clear();
            bestLocators.push_back(iter->first);
            distance = d;
          }
        }
        if (bestLocators.size() > 0)
          gresp->setLocatorString(bestLocators[intuniform(0, bestLocators.size() - 1)].c_str());
      }

      if (!gcall->getOriginatorHandle().isUnspecified() && gcall->getOriginatorHandle() != thisNode) {
        mDHTFastDataCall *fcall= new mDHTFastDataCall();
        fcall->setKey(gcall->getKey());
        fcall->setTtl(gresp->getTtl());
        fcall->setSuccess(gresp->getSuccess());
        fcall->setOwnerString(gresp->getOwnerString());
        fcall->setLocatorString(gresp->getLocatorString());
        fcall->setVersion(gresp->getVersion());
        fcall->setOriginatorRpcId(gcall->getOriginatorRpcId());
        fcall->setLevel(gcall->getLevel());

        // XXX: Hack for an OverSim bug...
        TransportAddress addr = gcall->getOriginatorHandle();
        addr.setPort(overlay->getThisNode().getPort());

        sendRouteRpcCall(TIER1_COMP, addr, fcall, NULL);
      }

      sendRpcResponse(gcall, gresp);      
      return;
    }

    if (gcall->getLevel() < numLevels - 1) {
      GetMapEntry getEntry;
      getEntry.icallMsg = gcall;
      getEntry.originator = gcall->getOriginator();
      getMap.insert(make_pair (gcall->getNonce(), getEntry));

      mDHTGetCall *gcall2 = new mDHTGetCall();
      gcall2->setKey(gcall->getKey());
      gcall2->setOriginator(gcall->getOriginator());
      gcall2->setOriginatorHandle(gcall->getOriginatorHandle());
      gcall2->setOriginatorRpcId(gcall->getOriginatorRpcId());
      gcall2->setLevel(gcall->getLevel()+1);
      
      sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, gcall2->getLevel(), 
         TransportAddress::UNSPECIFIED_NODE,
         gcall->getKey(), gcall2,
         new DHTStatsContext(globalStatistics->isMeasuring(),
              gcall->getKey()),
          DEFAULT_ROUTING, -1, 0, gcall->getNonce(), NULL);
    } else {
      mDHTGetResponse *gresp = new mDHTGetResponse();
      gresp->setKey(gcall->getKey());
      gresp->setSuccess(false);
      gresp->setLevel(gcall->getLevel());
      gresp->setPathArraySize(1);
      gresp->setPath(0, thisNode);

      if (!gcall->getOriginatorHandle().isUnspecified() && gcall->getOriginatorHandle() != thisNode) {
        mDHTFastDataCall *fcall= new mDHTFastDataCall();
        fcall->setKey(gcall->getKey());
        fcall->setTtl(gresp->getTtl());
        fcall->setSuccess(gresp->getSuccess());
        fcall->setOwnerString(gresp->getOwnerString());
        fcall->setLocatorString(gresp->getLocatorString());
        fcall->setVersion(gresp->getVersion());
        fcall->setOriginatorRpcId(gcall->getOriginatorRpcId());
        fcall->setLevel(gcall->getLevel());

        // XXX: Hack for an OverSim bug...
        TransportAddress addr = gcall->getOriginatorHandle();
        addr.setPort(overlay->getThisNode().getPort());

        sendRouteRpcCall(TIER1_COMP, addr, fcall, NULL);
      }

      sendRpcResponse(gcall, gresp);      
    }
}

void mDHT::handleGetRequest(mDHTGetCall* gcall) {
  delayedCalls.insert(std::make_pair(gcall->getNonce(), gcall));

  if (gcall->getCreationTime() != simTime() && gcall->getLevel() > 0 && perLevelDelayFactor[gcall->getLevel()-1] > 1.0) {
    mDHTDelayMessage *dmsg = new mDHTDelayMessage();

    dmsg->setDelayedType(1);
    dmsg->setRpcId(gcall->getNonce());
    dmsg->setRequest(true);

    scheduleAt(simTime() + (perLevelDelayFactor[gcall->getLevel()-1] - 1.0) * (simTime() - gcall->getCreationTime()), dmsg);
  } else {
    handleInternalGetRequest(gcall->getNonce());
  }
}


void mDHT::handleGetAPIRequest(NetInfAPIGetCall* gcall) {
	EV << "mDHT: GET API CALL for key " << gcall->getKey() << " arrived \n";

    RECORD_STATS(numGetRequests++);

    std::ostringstream os;
    os << stringName << ":" << gcall->getOriginatorUser();

    RECORD_STATS(numInternalGetRequests++; numSampledGets++);
    DataStorageEntry *entry = dataStorage.get(gcall->getKey());

    if (entry) {
      if (entry->isCopy)
        RECORD_STATS(numCopyHits++; numSampledCopyHits++);
      RECORD_STATS(numHits++; numSampledHits++);
      RECORD_STATS(numSuccGet++);

      NetInfAPIGetResponse* gresp = new NetInfAPIGetResponse();
      gresp->setKey(entry->key);
      gresp->setTtl((entry->ttlMessage ? (int)SIMTIME_DBL(entry->ttlMessage->getArrivalTime() - simTime()) : cacheTtl));
      gresp->setSuccess(true);
      gresp->setOwnerString(entry->ownerString.c_str());
      gresp->setVersion(entry->version);

      {
        int distance = -1;
        std::string bestLocator;
        std::map<std::string, std::pair<int, bool> >::const_iterator iter;
        std::string originator = os.str();

        for (iter = entry->secondaryLocators.begin(); iter != entry->secondaryLocators.end(); iter++) {
          if (iter->second.first < entry->version)
            continue;

          int n = std::min(iter->first.length(), originator.length());
          int d = 0;
          for (int i = 0; i < n; i++) {
            if (iter->first[i] != originator[i])
              break;
            if (iter->first[i] == '.')
              d++;
          }
          if (iter->first == originator) {
            bestLocator = iter->first;
            distance = d;
            break;
          } else if (d > distance) {
            bestLocator = iter->first;
            distance = d;
          }
        }
        gresp->setLocatorString(bestLocator.c_str());
      }
      
      RECORD_STATS(globalStatistics->addStdDev("mDHT: Used Levels", 0));
      if (enableTimeStatistics) {
        RECORD_STATS(globalStatistics->recordOutVector("mDHT: Used Levels", 0));
        RECORD_STATS(globalStatistics->recordHistogram("mDHT: Used Levels", 0));
      }
      
      sendRpcResponse(gcall, gresp);      
      return;
    }

    GetMapEntry getEntry;
    getEntry.acallMsg = gcall;

    getMap.insert(make_pair(gcall->getNonce(), getEntry));

    mDHTGetCall *gcall2 = new mDHTGetCall();
    gcall2->setKey(gcall->getKey());
    gcall2->setLevel(0);
    {
      std::ostringstream os;
      os << stringName << ":" << gcall->getOriginatorUser();
      gcall2->setOriginator(os.str().c_str());
    }
    gcall2->setOriginatorHandle(thisNode);
    gcall2->setOriginatorRpcId(gcall->getNonce());

    // request an unterste Ebene schicken
    sendRpcCallToLevel(ROUTE_TRANSPORT, TIER1_COMP, 0,
        TransportAddress::UNSPECIFIED_NODE,
        gcall->getKey(), gcall2,
        new DHTStatsContext(globalStatistics->isMeasuring(),
            gcall->getKey()),
        DEFAULT_ROUTING, -1, 0, gcall->getNonce(), NULL);
}

void mDHT::handleRpcResponse(BaseResponseMessage* msg,
                                   cPolymorphic* context, int rpcId,
                                   simtime_t rtt)
{
    RPC_SWITCH_START(msg)
    RPC_ON_RESPONSE( NetInfAPIGet )
    {
        assert(false); // must not happen
        break;
    }
    RPC_ON_RESPONSE( NetInfAPIPut )
    {
        assert(false); // must not happen
        break;
    }
    RPC_ON_RESPONSE( NetInfAPIAddLocator )
    {
        assert(false); // must not happen
        break;
    }
    RPC_ON_RESPONSE( NetInfAPIRemoveLocator )
    {
        assert(false); // must not happen
        break;
    }
    RPC_ON_RESPONSE( mDHTPut )
    {
        handlePutResponse(_mDHTPutResponse,
                          check_and_cast<DHTStatsContext*>(context),
                          rtt, rpcId);
        EV << "[mDHT::handleRpcResponse()]\n"
           << "    Put RPC Response received: id=" << rpcId
           << " msg=" << *_mDHTPutResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_ON_RESPONSE( mDHTGet)
    {
        handleGetResponse(_mDHTGetResponse,
                          check_and_cast<DHTStatsContext*>(context), rtt,
                          rpcId);

        EV << "[mDHT::handleRpcResponse()]\n"
           << "    Get RPC Response received: id=" << rpcId
           << " msg=" << *_mDHTGetResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_ON_RESPONSE( mDHTAddLocator)
    {
        handleAddLocatorResponse(_mDHTAddLocatorResponse,
                          check_and_cast<DHTStatsContext*>(context), rtt,
                          rpcId);

        EV << "[mDHT::handleRpcResponse()]\n"
           << "    AddLocator RPC Response received: id=" << rpcId
           << " msg=" << *_mDHTAddLocatorResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_ON_RESPONSE( mDHTRemoveLocator)
    {
        handleRemoveLocatorResponse(_mDHTRemoveLocatorResponse,
                          check_and_cast<DHTStatsContext*>(context), rtt,
                          rpcId);

        EV << "[mDHT::handleRpcResponse()]\n"
           << "    RemoveLocator RPC Response received: id=" << rpcId
           << " msg=" << *_mDHTRemoveLocatorResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_SWITCH_END()
}

void mDHT::handleInternalPutResponse(int rpcId, const OverlayKey& key, bool success, const std::list<TransportAddress>& path)
{
    // Response an tier 2 weiterleiten
    UNORDERED_MAP<int, PutMapEntry>::iterator it3 = putMap.find(rpcId);
    if (it3 == putMap.end())
      return;

        if (it3->second.icallMsg != 0) {
          // Response erstellen
          mDHTPutResponse* presp = new mDHTPutResponse();
          presp->setKey(key);
          presp->setSuccess(success);
          presp->setLevel(it3->second.icallMsg->getLevel());
          
          unsigned int n = path.size(), i;
          std::list<TransportAddress>::const_iterator piter;
          presp->setPathArraySize(n + 1);
          for (piter = path.begin(), i = 0; piter != path.end(); piter++, i++)
            presp->setPath(i, *piter);
          presp->setPath(i, thisNode);

          sendRpcResponse(it3->second.icallMsg, presp);
        } else if (it3->second.acallMsg != 0) {
#if 0
          int pathLength = -1, virtualPathLength = -1;
          unsigned int n;
          const TransportAddress *last = 0, *current;
          bool onlySelf = true;
          
          std::list<TransportAddress>::const_iterator piter;
          for (piter = path.begin(); piter != path.end(); piter++) {
            current = &(*piter);
            onlySelf = onlySelf && (*current == thisNode);
            if (last == NULL || *last != *current)
              pathLength++;
            virtualPathLength++;
            last = current;
          }
          if (onlySelf)
            pathLength = virtualPathLength = 0;
          else if (numLevels == 1)
            pathLength = virtualPathLength = 1;
#endif

          // Response erstellen
          NetInfAPIPutResponse* presp = new NetInfAPIPutResponse();
          presp->setKey(key);
          presp->setSuccess(success);
          presp->setOwnerString(it3->second.ownerString.c_str());
          
          if (success) {
            RECORD_STATS(numSuccPut++);
          } else {
            RECORD_STATS(numFailPut++);
          }

          sendRpcResponse(it3->second.acallMsg, presp);
        }
        putMap.erase(it3);
}

void mDHT::handlePutResponse(mDHTPutResponse* msg, DHTStatsContext* context, simtime_t rtt, int rpcId) {
    if (msg->getCreationTime() != simTime() && msg->getLevel() > 0 && perLevelDelayFactor[msg->getLevel() - 1] > 1.0) {
      mDHTDelayMessage *dmsg = new mDHTDelayMessage();
      double d = perLevelDelayFactor[msg->getLevel()-1];
      dmsg->setDelayedType(0);
      dmsg->setRequest(false);
      dmsg->setRpcId(rpcId);
      dmsg->setSuccess(msg->getSuccess());
      dmsg->setKey(msg->getKey());
      
      int n = msg->getPathArraySize(), i;
      dmsg->setPathArraySize(n);
      for (i = 0; i < n; i++)
        dmsg->setPath(i, msg->getPath(i));

      scheduleAt(simTime() + (d - 1.0) * (simTime() - msg->getCreationTime()), dmsg);
    } else {
      std::list<TransportAddress> path;
      int n = msg->getPathArraySize(), i;
      for (i = 0; i < n; i++)
        path.push_back(msg->getPath(i));

      handleInternalPutResponse(rpcId, msg->getKey(), msg->getSuccess(), path);
    }
    delete context;
}

void mDHT::handleInternalAddLocatorResponse(int rpcId, const OverlayKey& key, bool success, const std::list<TransportAddress>& path)
{
    // Response an tier 2 weiterleiten
    UNORDERED_MAP<int, AddMapEntry>::iterator it3 = addMap.find(rpcId);
    if (it3 == addMap.end())
      return;

        if (it3->second.icallMsg != 0) {
          // Response erstellen
          mDHTAddLocatorResponse* aresp = new mDHTAddLocatorResponse();
          aresp->setKey(key);
          aresp->setSuccess(success);
          aresp->setLevel(it3->second.icallMsg->getLevel());

          unsigned int n = path.size(), i;
          std::list<TransportAddress>::const_iterator piter;
          aresp->setPathArraySize(n + 1);
          for (piter = path.begin(), i = 0; piter != path.end(); piter++, i++)
            aresp->setPath(i, *piter);
          aresp->setPath(i, thisNode);

          sendRpcResponse(it3->second.icallMsg, aresp);
        } else if (it3->second.acallMsg != 0) {
#if 0
          int pathLength = -1, virtualPathLength = -1;
          unsigned int n;
          const TransportAddress *last = 0, *current;
          bool onlySelf = true;
          
          std::list<TransportAddress>::const_iterator piter;
          for (piter = path.begin(); piter != path.end(); piter++) {
            current = &(*piter);
            onlySelf = onlySelf && (*current == thisNode);
            if (last == NULL || *last != *current)
              pathLength++;
            virtualPathLength++;
            last = current;
          }
          if (onlySelf)
            pathLength = virtualPathLength = 0;
          else if (numLevels == 1)
            pathLength = virtualPathLength = 1;
#endif

          // Response erstellen
          NetInfAPIAddLocatorResponse* aresp = new NetInfAPIAddLocatorResponse();
          aresp->setKey(key);
          aresp->setSuccess(success);
          aresp->setLocatorString(it3->second.locatorString.c_str());

          if (success) {
            RECORD_STATS(numSuccAdd++);
          } else {
            RECORD_STATS(numFailAdd++);
          }

          sendRpcResponse(it3->second.acallMsg, aresp);
        }
        addMap.erase(it3);
}

void mDHT::handleAddLocatorResponse(mDHTAddLocatorResponse* msg, DHTStatsContext* context, simtime_t rtt, int rpcId) {
    if (msg->getCreationTime() != simTime() && msg->getLevel() > 0 && perLevelDelayFactor[msg->getLevel() - 1] > 1.0) {
      mDHTDelayMessage *dmsg = new mDHTDelayMessage();
      double d = perLevelDelayFactor[msg->getLevel()-1];
      dmsg->setDelayedType(2);
      dmsg->setRequest(false);
      dmsg->setRpcId(rpcId);
      dmsg->setSuccess(msg->getSuccess());
      dmsg->setKey(msg->getKey());

      int n = msg->getPathArraySize(), i;
      dmsg->setPathArraySize(n);
      for (i = 0; i < n; i++)
        dmsg->setPath(i, msg->getPath(i));

      scheduleAt(simTime() + (d - 1.0) * (simTime() - msg->getCreationTime()), dmsg);
    } else {
      std::list<TransportAddress> path;
      int n = msg->getPathArraySize(), i;
      for (i = 0; i < n; i++)
        path.push_back(msg->getPath(i));

      handleInternalAddLocatorResponse(rpcId, msg->getKey(), msg->getSuccess(), path);
    }
    delete context;
}

void mDHT::handleInternalRemoveLocatorResponse(int rpcId, const OverlayKey& key, bool success, const std::list<TransportAddress>& path)
{
    // Response an tier 2 weiterleiten
    UNORDERED_MAP<int, RemoveMapEntry>::iterator it3 = removeMap.find(rpcId);
    if (it3 == removeMap.end())
      return;

        if (it3->second.icallMsg != 0) {
          // Response erstellen
          mDHTRemoveLocatorResponse* rresp = new mDHTRemoveLocatorResponse();
          rresp->setKey(key);
          rresp->setSuccess(success);
          rresp->setLevel(it3->second.icallMsg->getLevel());

          unsigned int n = path.size(), i;
          std::list<TransportAddress>::const_iterator piter;
          rresp->setPathArraySize(n + 1);
          for (piter = path.begin(), i = 0; piter != path.end(); piter++, i++)
            rresp->setPath(i, *piter);
          rresp->setPath(i, thisNode);

          sendRpcResponse(it3->second.icallMsg, rresp);
        } else if (it3->second.acallMsg != 0) {
#if 0
          int pathLength = -1, virtualPathLength = -1;
          unsigned int n;
          const TransportAddress *last = 0, *current;
          bool onlySelf = true;
          
          std::list<TransportAddress>::const_iterator piter;
          for (piter = path.begin(); piter != path.end(); piter++) {
            current = &(*piter);
            onlySelf = onlySelf && (*current == thisNode);
            if (last == NULL || *last != *current)
              pathLength++;
            virtualPathLength++;
            last = current;
          }
          if (onlySelf)
            pathLength = virtualPathLength = 0;
          else if (numLevels == 1)
            pathLength = virtualPathLength = 1;
#endif

          // Response erstellen
          NetInfAPIRemoveLocatorResponse* rresp = new NetInfAPIRemoveLocatorResponse();
          rresp->setKey(key);
          rresp->setSuccess(success);
          rresp->setLocatorString(it3->second.locatorString.c_str());
          if (success) {
            RECORD_STATS(numSuccRemove++);
          } else {
            RECORD_STATS(numFailRemove++);
          }

          sendRpcResponse(it3->second.acallMsg, rresp);
        }
        removeMap.erase(it3);
}

void mDHT::handleRemoveLocatorResponse(mDHTRemoveLocatorResponse* msg, DHTStatsContext* context, simtime_t rtt, int rpcId) {
    if (msg->getCreationTime() != simTime() && msg->getLevel() > 0 && perLevelDelayFactor[msg->getLevel() - 1] > 1.0) {
      mDHTDelayMessage *dmsg = new mDHTDelayMessage();
      double d = perLevelDelayFactor[msg->getLevel()-1];
      dmsg->setDelayedType(3);
      dmsg->setRequest(false);
      dmsg->setRpcId(rpcId);
      dmsg->setSuccess(msg->getSuccess());
      dmsg->setKey(msg->getKey());

      int n = msg->getPathArraySize(), i;
      dmsg->setPathArraySize(n);
      for (i = 0; i < n; i++)
        dmsg->setPath(i, msg->getPath(i));

      scheduleAt(simTime() + (d - 1.0) * (simTime() - msg->getCreationTime()), dmsg);
    } else {
      std::list<TransportAddress> path;
      int n = msg->getPathArraySize(), i;
      for (i = 0; i < n; i++)
        path.push_back(msg->getPath(i));
    
      handleInternalRemoveLocatorResponse(rpcId, msg->getKey(), msg->getSuccess(), path);
    }
    delete context;
}

void mDHT::handleInternalGetResponse(int rpcId, const OverlayKey& key, int ttl, bool success, const std::string& ownerString, const std::string& locatorString, int version, const std::list<TransportAddress>& path)
{
	EV << "handleGetResponse: Result is " << success << endl;

    UNORDERED_MAP<int, GetMapEntry>::iterator it3 = getMap.find(rpcId);
    if (it3 == getMap.end())
      return;

/*
        {
          std::ostringstream os;
          os << "mDHT: Latency at level " << it3->second.lastUsedLevel + 1;
          RECORD_STATS(globalStatistics->addStdDev(os.str(), SIMTIME_DBL(rtt)));
        }
*/

    if (success) {
      if (it3 != getMap.end() && passiveCache && ttl > 0) {
        dataStorage.add(key, ownerString, version, true, true, ttl);
        if (locatorString != "") {
          dataStorage.addSecondaryLocator(key, locatorString, version, true);
        }
      } else if (it3 == getMap.end()) {
        // refresh GET
        DataStorageEntry* entry = dataStorage.get(key);
        if (entry)
          dataStorage.add(key, ownerString, version, true, false, copyRefreshTime);
      }
    }


    if (it3->second.icallMsg) {
          // Response erstellen
          mDHTGetResponse* gresp = new mDHTGetResponse();
          gresp->setKey(key);
          gresp->setTtl(ttl);
          gresp->setSuccess(success);
          gresp->setOwnerString(ownerString.c_str());
          gresp->setLocatorString(locatorString.c_str());
          gresp->setVersion(version);
          gresp->setLevel(it3->second.icallMsg->getLevel());

          unsigned int n = path.size(), i;
          std::list<TransportAddress>::const_iterator piter;
          gresp->setPathArraySize(n + 1);
          for (piter = path.begin(), i = 0; piter != path.end(); piter++, i++)
            gresp->setPath(i, *piter);
          gresp->setPath(i, thisNode);

          sendRpcResponse(it3->second.icallMsg, gresp);
    } else {
          int pathLength = -1, virtualPathLength = -1;
          unsigned int n;
          const TransportAddress *last = 0, *current;
          bool onlySelf = true;
          
          std::list<TransportAddress>::const_iterator piter;
          for (piter = path.begin(); piter != path.end(); piter++) {
            current = &(*piter);
            onlySelf = onlySelf && (*current == thisNode);
            if (last == NULL || *last != *current)
              pathLength++;
            virtualPathLength++;
            last = current;
          }
          if (onlySelf)
            pathLength = virtualPathLength = 0;
          else if (numLevels == 1)
            pathLength = virtualPathLength = 1;

          RECORD_STATS(globalStatistics->addStdDev("mDHT: Used Levels", pathLength));
          if (enableTimeStatistics) {
            RECORD_STATS(globalStatistics->recordOutVector("mDHT: Used Levels", pathLength));
            RECORD_STATS(globalStatistics->recordHistogram("mDHT: Used Levels", pathLength));
          }

          if (success) {
            RECORD_STATS(numSuccGet++);
          } else {
            RECORD_STATS(numFailGet++);
          }

          // Response erstellen
          if (it3->second.acallMsg) {
            NetInfAPIGetResponse* gresp = new NetInfAPIGetResponse();
            gresp->setKey(key);
            gresp->setTtl(ttl);
            gresp->setSuccess(success);
            gresp->setOwnerString(ownerString.c_str());
            gresp->setLocatorString(locatorString.c_str());
            gresp->setVersion(version);

            sendRpcResponse(it3->second.acallMsg, gresp);
            it3->second.acallMsg = NULL;
          }
    }

    getMap.erase(it3);
}

void mDHT::handleGetResponse(mDHTGetResponse* msg, DHTStatsContext* context, simtime_t rtt, int rpcId) {
        if (msg->getCreationTime() != simTime() && msg->getLevel() > 0 && perLevelDelayFactor[msg->getLevel() - 1] > 1.0) {
          mDHTDelayMessage *dmsg = new mDHTDelayMessage();
          double d = perLevelDelayFactor[msg->getLevel()-1];
          dmsg->setDelayedType(1);
          dmsg->setRequest(false);
          dmsg->setRpcId(rpcId);
          dmsg->setKey(msg->getKey());
          dmsg->setSuccess(msg->getSuccess());
          dmsg->setOwnerString(msg->getOwnerString());
          dmsg->setLocatorString(msg->getLocatorString());
          dmsg->setVersion(msg->getVersion());
          dmsg->setTtl(msg->getTtl());
          
          int n = msg->getPathArraySize(), i;
          dmsg->setPathArraySize(n);
          for (i = 0; i < n; i++)
            dmsg->setPath(i, msg->getPath(i));
          
          scheduleAt(simTime() + (d - 1.0) * (simTime() - msg->getCreationTime()), dmsg);
        } else {
          std::list<TransportAddress> path;
          int n = msg->getPathArraySize(), i;
          for (i = 0; i < n; i++)
            path.push_back(msg->getPath(i));

          handleInternalGetResponse(rpcId, msg->getKey(), msg->getTtl(), msg->getSuccess(), msg->getOwnerString(), msg->getLocatorString(), msg->getVersion(), path);
        }
        delete context;
}

void mDHT::handleRpcTimeout(BaseCallMessage* msg,
                                 const TransportAddress& dest,
                                 cPolymorphic* context, int rpcId,
                                 const OverlayKey&) 
{
    RPC_SWITCH_START( msg )
      RPC_ON_CALL(NetInfAPIGet)
      {
          assert(false); // must not happen
          break;
      }
      RPC_ON_CALL(NetInfAPIPut)
      {
          assert(false); // must not happen
          break;
      }
      RPC_ON_CALL(NetInfAPIAddLocator)
      {
          assert(false); // must not happen
          break;
      }
      RPC_ON_CALL(NetInfAPIRemoveLocator)
      {
          assert(false); // must not happen
          break;
      }
      RPC_ON_CALL(mDHTGet)
      {
          handleInternalGetResponse(rpcId, _mDHTGetCall->getKey(), 0, false, "", "", 0, std::list<TransportAddress>());
          break;
      }
      RPC_ON_CALL(mDHTPut)
      {
          handleInternalPutResponse(rpcId, _mDHTPutCall->getKey(), false, std::list<TransportAddress>());
          break;
      }
      RPC_ON_CALL(mDHTAddLocator)
      {
          handleInternalAddLocatorResponse(rpcId, _mDHTAddLocatorCall->getKey(), false, std::list<TransportAddress>());
          break;
      }
      RPC_ON_CALL(mDHTRemoveLocator)
      {
          handleInternalRemoveLocatorResponse(rpcId, _mDHTRemoveLocatorCall->getKey(), false, std::list<TransportAddress>());
          break;
      }
    RPC_SWITCH_END( )
}

void mDHT::finishApp() {

  UNORDERED_MAP<int, GetMapEntry>::iterator giter;
  for (giter = getMap.begin(); giter != getMap.end(); giter++) {
    cancelAndDelete(giter->second.icallMsg);
    cancelAndDelete(giter->second.acallMsg);
  }
  getMap.clear();

  UNORDERED_MAP<int, PutMapEntry>::iterator piter;
  for (piter = putMap.begin(); piter != putMap.end(); piter++) {
    cancelAndDelete(piter->second.icallMsg);
    cancelAndDelete(piter->second.acallMsg);
  }
  putMap.clear();

  UNORDERED_MAP<int, AddMapEntry>::iterator aiter;
  for (aiter = addMap.begin(); aiter != addMap.end(); aiter++) {
    cancelAndDelete(aiter->second.icallMsg);
    cancelAndDelete(aiter->second.acallMsg);
  }
  addMap.clear();

  UNORDERED_MAP<int, RemoveMapEntry>::iterator riter;
  for (riter = removeMap.begin(); riter != removeMap.end(); riter++) {
    cancelAndDelete(riter->second.icallMsg);
    cancelAndDelete(riter->second.acallMsg);
  }
  removeMap.clear();

  DelayedCalls::iterator diter;
  for (diter = delayedCalls.begin(); diter != delayedCalls.end(); diter++)
    cancelAndDelete(diter->second);
  delayedCalls.clear();

  globalStatistics->addStdDev("mDHT: Received Get Requests", numGetRequests);
  globalStatistics->addStdDev("mDHT: Successful Get Requests", numSuccGet);
  globalStatistics->addStdDev("mDHT: Failed Get Requests", numFailGet);
  globalStatistics->addStdDev("mDHT: Received Put Requests", numPutRequests);
  globalStatistics->addStdDev("mDHT: Successful Put Requests", numSuccPut);
  globalStatistics->addStdDev("mDHT: Failed Put Requests", numFailPut);
  globalStatistics->addStdDev("mDHT: Received AddLocator Requests", numAddRequests);
  globalStatistics->addStdDev("mDHT: Successful AddLocator Requests", numSuccAdd);
  globalStatistics->addStdDev("mDHT: Failed AddLocator Requests", numFailAdd);
  globalStatistics->addStdDev("mDHT: Received RemoveLocator Requests", numRemoveRequests);
  globalStatistics->addStdDev("mDHT: Successful RemoveLocator Requests", numSuccRemove);
  globalStatistics->addStdDev("mDHT: Failed RemoveLocator Requests", numFailRemove);

  globalStatistics->addStdDev("mDHT: Number of requests", numRequests);

  if (enableTimeStatistics) {
    globalStatistics->recordHistogram("mDHT: Number of requests", numRequests);
  }

  if (numGetRequests > 0) {
    globalStatistics->addStdDev("mDHT: Copy hit ratio", numCopyHits / ((double) numInternalGetRequests));
    globalStatistics->addStdDev("mDHT: Hit ratio", numHits / ((double) numInternalGetRequests));
  }

  cancelAndDelete(statisticsTimer);
}

mDHT::DataStorage::DataStorage(cSimpleModule *module) : module(module) {}

mDHT::DataStorageEntry* mDHT::DataStorage::get(const OverlayKey& key)
{
  EntryMap::iterator it = entries.find(key);
  if (it == entries.end())
    return NULL;

  if (it->second.isCache && maxCacheSize > 0) {
    std::list<OverlayKey>::iterator it2;
    for (it2 = lru.begin(); it2 != lru.end(); it2++) {
      if (*it2 == key) {
        lru.erase(it2);
        break;
      }
    }
    lru.push_back(key);
  }

  return &it->second;
}

void mDHT::DataStorage::add(const OverlayKey& key, const std::string& ownerString, int version, bool isCopy, bool isCache, int ttl)
{
  assert(ownerString != "");
  
  DataStorageEntry* entry = get(key);
  
  mDHTTtlMessage *ttlMessage = NULL;
  if (ttl != 0 && (!entry || (entry->isCache || entry->isCopy)) && (isCache || isCopy)) {
    ttlMessage = new mDHTTtlMessage();
    ttlMessage->setKey(key);
    module->scheduleAt(simTime() + ttl, ttlMessage);
  }

  if (maxCacheSize > 0 && !entry && isCache && lru.size() >= (unsigned int) maxCacheSize) {
    remove(lru.front());
  }

  if (entry) {
    assert(entry->ownerString == ownerString);
    if (entry->ttlMessage)
      module->cancelAndDelete(entry->ttlMessage);
    entry->ttlMessage = ttlMessage;

    if (entry->version < version)
      entry->version = version;

    if (!isCopy)
      entry->isCopy = false;

    if (!isCache) {
      if (entry->isCache) {
        std::list<OverlayKey>::iterator it2;
        for (it2 = lru.begin(); it2 != lru.end(); it2++) {
          if (*it2 == key) {
            lru.erase(it2);
            break;
          }
        }        
        entry->secondaryLocators.clear();
      }

      entry->isCache = false;
    }
  } else {
    entries.insert(std::make_pair(key, DataStorageEntry(key, ownerString, version, isCopy, isCache, ttlMessage)));
    if (maxCacheSize > 0 && isCache)
      lru.push_back(key);
  }
}

void mDHT::DataStorage::remove(const OverlayKey& key)
{
  EntryMap::iterator it = entries.find(key);
  if (it == entries.end())
    return;

  if (it->second.ttlMessage) {
    module->cancelAndDelete(it->second.ttlMessage);
  }

  if (it->second.isCache) {
    std::list<OverlayKey>::iterator it2;
    for (it2 = lru.begin(); it2 != lru.end(); it2++) {
      if (*it2 == key) {
        lru.erase(it2);
        break;
      }
    }
  }

  entries.erase(it);
}

void mDHT::DataStorage::clear()
{
  EntryMap::iterator it;
  for (it = entries.begin(); it != entries.end(); it++)
    module->cancelAndDelete(it->second.ttlMessage);
  entries.clear();
  lru.clear();
}

void mDHT::DataStorage::setMaxCacheSize(int size)
{
  maxCacheSize = size;
}

void mDHT::DataStorage::addSecondaryLocator(const OverlayKey& key, const std::string& locatorString, int version, bool isCache)
{
  DataStorageEntry* entry = get(key);
  
  if (!entry)
    return;

  if (!entry->isCache && isCache)
    return;
  else if (isCache)
    entry->secondaryLocators.clear();

  std::map<std::string, std::pair<int, bool> >::iterator iter = entry->secondaryLocators.find(locatorString);
  if (iter != entry->secondaryLocators.end()) {
    if (iter->second.first < version)
      iter->second.first = version;
    iter->second.second = isCache;
  } else {
    entry->secondaryLocators.insert(std::make_pair(locatorString, std::make_pair(version, isCache)));
  }
}

void mDHT::DataStorage::removeSecondaryLocator(const OverlayKey& key, const std::string& locatorString)
{
  DataStorageEntry* entry = get(key);

  if (!entry)
    return;

  entry->secondaryLocators.erase(locatorString);
}
