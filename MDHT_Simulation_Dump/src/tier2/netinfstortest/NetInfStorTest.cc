//
// Copyright (C) 2007 Institut fuer Telematik, Universitaet Karlsruhe (TH)
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
 * @file NetInfStorTest.cc
 * @author Ingmar Baumgart
 */

#include <IPAddressResolver.h>
#include <GlobalNodeListAccess.h>
#include <GlobalStatisticsAccess.h>
#include <UnderlayConfiguratorAccess.h>
#include <RpcMacros.h>
#include "CommonMessages_m.h"

#include "NetInfStorTest.h"
#include "NetInfStorTest_m.h"

Define_Module(NetInfStorTest);

using namespace std;

NetInfStorTest::~NetInfStorTest()
{
  cancelAndDelete(preseed_timer);
}

NetInfStorTest::NetInfStorTest()
{
  preseed_timer = NULL;
}

static int runningApps = 0;

static UNORDERED_MAP<std::string, int> clientRequests;
static UNORDERED_MAP<std::string, int> sampledClientRequests;
static std::string sampleApp = "";
static int numNodes = 0;

void NetInfStorTest::initializeApp(int stage)
{
  if (stage != MIN_STAGE_APP)
      return;

  runningApps++;

  // fetch parameters
  numNodes = par("targetOverlayTerminalNum");

  int usersPerNode = par("usersPerNode");
  users.resize(usersPerNode);

  versionLifetime = par("versionLifetime");
  copyLifetime = par("copyLifetime");
  publishUserRatio = par("publishUserRatio");
  publishCopyUserRatio = par("publishCopyUserRatio");

  statisticsSampleInterval = par("statisticsSampleInterval");
  enableTimeStatistics = par("enableTimeStatistics");

  globalNodeList = GlobalNodeListAccess().get();
  underlayConfigurator = UnderlayConfiguratorAccess().get();
  globalStatistics = GlobalStatisticsAccess().get();

  globalDataStorage = dynamic_cast<NetInfGlobalDataStorage*>(simulation.getModuleByPath(
      "globalObserver.globalFunctions[0].function"));

  if (globalDataStorage == NULL) {
    throw cRuntimeError("NetInfStorTest::initializeApp(): "
                        "NetInfGlobalDataStorage module not found!");
  }

  hostnameInfo = dynamic_cast<HostnameInfo*>(simulation.getModuleByPath(
            "globalObserver.globalFunctions[1].function"));
  assert(hostnameInfo != NULL);

  // initialize our statistics variables
  if (enableTimeStatistics) {
    statisticsTimer = new cMessage("statistics_timer");
    scheduleAt(simTime() + statisticsSampleInterval, statisticsTimer);
  } else {
    statisticsTimer = NULL;
  }

  // statistics
  numSent = 0;
  numGetSent = 0;
  numGetError = 0;
  numGetSuccess = 0;
  numPutSent = 0;
  numPutError = 0;
  numPutSuccess = 0;
  numModSent = 0;
  numModError = 0;
  numModSuccess = 0;

  WATCH(numSent);
  WATCH(numGetSent);
  WATCH(numGetError);
  WATCH(numGetSuccess);
  WATCH(numPutSent);
  WATCH(numPutError);
  WATCH(numPutSuccess);
  WATCH(numModSent);
  WATCH(numModError);
  WATCH(numModSuccess);

  nodeIsLeavingSoon = false;

  thisNode.setKey(OverlayKey::ZERO);
  thisNode.setPort(0);
  stringName = hostnameInfo->getHostname(thisNode.getAddress());
  assert(stringName != "");
 
  // init timers
  preseed_timer = new cMessage("preseed_timer");
  scheduleAt(simTime() + 100, preseed_timer);
}

void NetInfStorTest::handleRpcResponse(BaseResponseMessage* msg,
                                   const RpcState& state, simtime_t rtt)
{
  RPC_SWITCH_START(msg)
  RPC_ON_RESPONSE(NetInfAPIPut) {
    handlePutResponse(_NetInfAPIPutResponse,
                      check_and_cast<StatsContext*>(state.getContext()), rtt);
    EV << "[NetInfStorTest::handleRpcResponse()]\n"
       << "    NetInfAPIPut Response received: id=" << state.getId()
       << " msg=" << *_NetInfAPIPutResponse << " rtt=" << rtt
       << endl;
    break;
  }
  RPC_ON_RESPONSE(NetInfAPIGet)
  {
    handleGetResponse(_NetInfAPIGetResponse,
                      check_and_cast<StatsContext*>(state.getContext()), rtt);
    EV << "[NetInfStorTest::handleRpcResponse()]\n"
       << "   NetInfAPIGet Response received: id=" << state.getId()
       << " msg=" << *_NetInfAPIGetResponse << " rtt=" << rtt
       << endl;
    break;
  }
  RPC_ON_RESPONSE(NetInfAPIAddLocator)
  {
    handleAddLocatorResponse(_NetInfAPIAddLocatorResponse,
                      check_and_cast<StatsContext*>(state.getContext()), rtt);
    EV << "[NetInfStorTest::handleRpcResponse()]\n"
       << "   NetInfAPIAddLocator Response received: id=" << state.getId()
       << " msg=" << *_NetInfAPIAddLocatorResponse << " rtt=" << rtt
       << endl;
    break;
  }
  RPC_ON_RESPONSE(NetInfAPIRemoveLocator)
  {
    handleRemoveLocatorResponse(_NetInfAPIRemoveLocatorResponse,
                      check_and_cast<StatsContext*>(state.getContext()), rtt);
    EV << "[NetInfStorTest::handleRpcResponse()]\n"
       << "   NetInfAPIRemoveLocator Response received: id=" << state.getId()
       << " msg=" << *_NetInfAPIRemoveLocatorResponse << " rtt=" << rtt
       << endl;
    break;
  }
  RPC_SWITCH_END()
}

void NetInfStorTest::handleRpcTimeout(BaseCallMessage* msg,                     // called when an RPC times out
                          const TransportAddress& dest,
                          cPolymorphic* ctx, int rpcId,
                          const OverlayKey&)
{
  RPC_SWITCH_START(msg);        
    RPC_ON_CALL(NetInfAPIPut) {
      StatsContext *context = check_and_cast<StatsContext*>(ctx);

      handlePutResponse(NULL, context, simTime());
      break;
    }
    RPC_ON_CALL(NetInfAPIGet) {
      StatsContext *context = check_and_cast<StatsContext*>(ctx);

      handleUserGet(context->userIdx);
      handleGetResponse(NULL, context, simTime());
      break;
    }
    RPC_ON_CALL(NetInfAPIAddLocator) {
      StatsContext *context = check_and_cast<StatsContext*>(ctx);

      handleAddLocatorResponse(NULL, context, simTime());
      break;
    }
    RPC_ON_CALL(NetInfAPIRemoveLocator) {
      StatsContext *context = check_and_cast<StatsContext*>(ctx);

      handleRemoveLocatorResponse(NULL, context, simTime());
      break;
    }
  RPC_SWITCH_END();
}

void NetInfStorTest::handleUserGet(int userIdx)
{
  User *u = &users[userIdx];
  
  u->pendingClientRequests--;
  if (u->pendingClientRequests <= 0) {
    u->curUserRequest++;
    if (u->curUserRequest >= u->nUserRequests) {
      nextSession(userIdx);
    } else {
      nextRequest(userIdx);
    }
  }
}

void NetInfStorTest::nextRequest(int userIdx)
{
  NetInfStorTestUserMessage *umsg = new NetInfStorTestUserMessage("user_message");
  umsg->setStart(false);

  User *u = &users[userIdx];

  umsg->setUserIdx(userIdx);

  u->pendingClientRequests = -1;

  // A WEB BROWSING WORKLOAD MODEL FOR SIMULATION
  //   Lourens O. Walters
  // Table 6.14
  // Web User Request Inter-arrival time
  double requestDelay = std::max(std::min(weibull(106.420313, 0.85689), 900.0), 1.0);
  pendingDelayedMessages.insert(umsg);
  scheduleAt(simTime() + requestDelay, umsg);
}

void NetInfStorTest::nextSession(int userIdx, bool first)
{
  NetInfStorTestUserMessage *umsg = new NetInfStorTestUserMessage("user_message");
  umsg->setStart(false);

  User *u = &users[userIdx];
    
  umsg->setUserIdx(userIdx);

  // A WEB BROWSING WORKLOAD MODEL FOR SIMULATION
  //   Lourens O. Walters
  // Table 6.14
  // Number of Web User Requests per Browsing Session
  u->nUserRequests = std::min(std::max(lognormal(1.792736, 1.147842) + 1, 1.0), 99.0);
  u->curUserRequest = 0;
  u->pendingClientRequests = -1;

  double userSessionDelay;

  if (first) {
    // A WEB BROWSING WORKLOAD MODEL FOR SIMULATION
    //   Lourens O. Walters
    // Browsing Inter-Session time mean 81.04
    userSessionDelay = intuniform(0, 81.04);
  } else {
    // A WEB BROWSING WORKLOAD MODEL FOR SIMULATION
    //   Lourens O. Walters
    // Table 6.14
    // Browsing Inter-Session time
    userSessionDelay = std::min(std::max(gamma_d(102.342465, 0.645296) + 14, 1.0), 465.0);
  }

  pendingDelayedMessages.insert(umsg);
  scheduleAt(simTime() + userSessionDelay * 60, umsg);
}

void NetInfStorTest::nextPut(int userIdx)
{
  // start preseed PUT
  NetInfGlobalDataStorage::Entry *e = globalDataStorage->getRandomEntryForPut(thisNode, stringName, userIdx);
  if (!e) {
    if (globalDataStorage->isSeeding()) {
      NetInfStorTestUserMessage *umsg = new NetInfStorTestUserMessage("user_message");
      umsg->setStart(true);
      scheduleAt(simTime() + 10, umsg);      
    } else {
      startUsersGet();
    }
    return;
  }

  NetInfAPIPutCall* pcall = new NetInfAPIPutCall();
  pcall->setKey(e->key);
  pcall->setOwnerUser(userIdx);
  pcall->setVersion(0);

  RECORD_STATS(numSent++; numPutSent++);
  sendInternalRpcCall(TIER1_COMP, pcall,
      new StatsContext(globalStatistics->isMeasuring(),
          e, 0, userIdx));
}

void NetInfStorTest::startUsersPut()
{
  std::vector<User>::iterator iter;
  int i, n, N;

  n = publishUserRatio * users.size();
  N = publishCopyUserRatio * users.size();

  for (i = 0, iter = users.begin(); iter != users.end(); iter++, i++) {
    User *u = &users[i];
    u->idx = i;
    u->publish = (i < N);
    if (i < n)
      nextPut(i);
  }
}

void NetInfStorTest::startUsersGet()
{
  std::vector<User>::iterator iter;
  int i, N;

  N = publishCopyUserRatio * users.size();

  for (i = 0, iter = users.begin(); iter != users.end(); iter++, i++) {
    User *u = &users[i];
    u->idx = i;
    u->publish = (i < N);
    nextSession(i, true);
  }
}

void NetInfStorTest::handlePutResponse(NetInfAPIPutResponse* msg, StatsContext* context, simtime_t rtt)
{
  if (msg && msg->getSuccess())
    globalDataStorage->notifyStored(thisNode, msg->getOwnerString(), context->entry->key);

  if (msg && msg->getSuccess()) {
    User* u = &users[context->userIdx];
    u->ownedKeys.insert(context->entry->key);
  }

  // If this was a preseed PUT do the next one
  if (context->version == 0 && globalDataStorage->isSeeding()) {
    nextPut(context->userIdx);
  } else if (context->version == 0) {
    // If this was a preseed PUT but we're not preseeding anymore start normal operation
    startUsersGet();
  }

  if (versionLifetime > 0) {
    NetInfStorTestUpdateEntryMessage* uemsg = new NetInfStorTestUpdateEntryMessage("update_entry");
    uemsg->setKey(msg->getKey());
    uemsg->setUserIdx(context->userIdx);
    
    pendingDelayedMessages.insert(uemsg);
    scheduleAt(simTime() + 120 + exponential(std::max(versionLifetime - 120, 0)), uemsg);
  }

  if (context->measurementPhase == false) {    
    // don't count response, if the request was not sent
    // in the measurement phase
    delete context;
    return;
  }

  if (msg && msg->getSuccess()) {
    if (context->version == 0) {
      RECORD_STATS(numModSuccess++);
      RECORD_STATS(globalStatistics->addStdDev("NetInfStorTest: modification PUT Latency (s)",
                   SIMTIME_DBL(rtt)));
    } else {
      RECORD_STATS(numPutSuccess++);
      RECORD_STATS(globalStatistics->addStdDev("NetInfStorTest: PUT Latency (s)",
                   SIMTIME_DBL(rtt)));
    }
    if (enableTimeStatistics)
      globalStatistics->recordOutVector("NetInfStorTest: PUT latency", SIMTIME_DBL(rtt));
  } else {
    if (context->version == 0)
      RECORD_STATS(numModError++);
    else
      RECORD_STATS(numPutError++);
  }
  
  delete context;
}

void NetInfStorTest::handleAddLocatorResponse(NetInfAPIAddLocatorResponse* msg, StatsContext* context, simtime_t rtt)
{
  if (context->measurementPhase == false) {    
    // don't count response, if the request was not sent
    // in the measurement phase
    delete context;
    return;
  }

  // TODO: stats?

  delete context;
}

void NetInfStorTest::handleRemoveLocatorResponse(NetInfAPIRemoveLocatorResponse* msg, StatsContext* context, simtime_t rtt)
{
  if (context->measurementPhase == false) {    
    // don't count response, if the request was not sent
    // in the measurement phase
    delete context;
    return;
  }

  // TODO: stats?

  delete context;
}

std::vector<std::string>* stringSplit(const std::string& str, const std::string& d)
{
  std::vector<std::string>* ret = new std::vector<std::string>();
  int n = str.length(), dn = d.length();
  int i, j, l = 0;

  for (i = 0; i < n; i++) {
    bool match = false;
    
    for (j = 0; j < dn; j++) {
      if (str[i] == d[j]) {
        match = true;
        break;
      }
    }

    if (match) {
      ret->push_back(str.substr(l, i - l));
      l = i + 1;
    } else if (i == n - 1) {
      ret->push_back(str.substr(l, i - l + 1));
    }
  }

  return ret;
}

static int
getLocatorDistance(const std::string& a, const std::string& b)
{
  std::vector<std::string>* aParts = stringSplit(a, ".:");
  std::vector<std::string>* bParts = stringSplit(b, ".:");
  
  assert(aParts->size() == bParts->size());
  std::vector<std::string>::const_iterator iter1, iter2;
  
  int d = 0;
  
  for (iter1 = aParts->begin(), iter2 = bParts->begin(); iter1 != aParts->end() && iter2 != bParts->end(); iter1++, iter2++) {
    if (d != 0 || *iter1 != *iter2)
      d++;
  }
  
  delete aParts;
  delete bParts;
  
  return d;
}

void NetInfStorTest::handleGetResponse(NetInfAPIGetResponse* msg, StatsContext* context, simtime_t rtt)
{
  if (context->measurementPhase == false) {
    // don't count response, if the request was not sent
    // in the measurement phase
    
    handleUserGet(context->userIdx);

    delete context;
    return;
  }

  RECORD_STATS(globalStatistics->addStdDev("NetInfStorTest: GET Latency (s)",
               SIMTIME_DBL(rtt)));

  if (enableTimeStatistics) {
    globalStatistics->recordOutVector("NetInfStorTest: GET latency", SIMTIME_DBL(rtt));
    globalStatistics->recordHistogram("NetInfStorTest: GET latency", SIMTIME_DBL(rtt));
  }

  if (!msg || !msg->getSuccess()) {
    RECORD_STATS(numGetError++);

    handleUserGet(context->userIdx);
  } else {
    User *u = &users[context->userIdx];

    RECORD_STATS(numGetSuccess++);

    {
      std::ostringstream os;
      os << stringName << ":" << context->userIdx;
      const std::string& thisString = os.str();
      const std::string& locatorString = (strlen(msg->getLocatorString()) == 0) ? msg->getOwnerString() : msg->getLocatorString();

      int d = getLocatorDistance(thisString, locatorString);

      UNORDERED_MAP<std::string, int>::iterator iter = clientRequests.find(locatorString);
      if (iter == clientRequests.end())
        iter = clientRequests.insert(std::make_pair(locatorString, 0)).first;
      iter->second++;

      if (enableTimeStatistics) {
        iter = sampledClientRequests.find(locatorString);
        if (iter == sampledClientRequests.end())
          iter = sampledClientRequests.insert(std::make_pair(locatorString, 0)).first;
        iter->second++;

        if (sampleApp == "")
          sampleApp = stringName;
      }

      RECORD_STATS(globalStatistics->addStdDev("NetInfStorTest: Locator distance", d));
      if (enableTimeStatistics) {
        globalStatistics->recordOutVector("NetInfStorTest: Locator distance", d);
        globalStatistics->recordHistogram("NetInfStorTest: Locator distance", d);
      }
    }

    int expected = context->entry->version;
    int version = msg->getVersion();
    RECORD_STATS(globalStatistics->addStdDev("NetInfStorTest: Version difference", expected - version));

    if (u->publish && copyLifetime > 0) {
      UNORDERED_MAP<OverlayKey, std::pair<int, cMessage*> >::iterator citer;
      citer = u->copiedKeys.find(msg->getKey());
      if (citer != u->copiedKeys.end()) {
        if (citer->second.first < version) {
          citer->second.first = version;
        
          NetInfAPIAddLocatorCall *alcall = new NetInfAPIAddLocatorCall();
          alcall->setKey(msg->getKey());
          alcall->setOwnerString(context->entry->ownerString.c_str());
          alcall->setLocatorUser(context->userIdx);
          alcall->setVersion(version);
          sendInternalRpcCall(TIER1_COMP, alcall,
              new StatsContext(globalStatistics->isMeasuring(),
                  context->entry, version, context->userIdx));
        }

        pendingDelayedMessages.erase(citer->second.second);
        cancelAndDelete(citer->second.second);
        NetInfStorTestRemoveLocatorMessage *rlmsg = new NetInfStorTestRemoveLocatorMessage("remove_locator");
        rlmsg->setKey(msg->getKey());
        rlmsg->setUserIdx(context->userIdx);
        citer->second.second = rlmsg;
        pendingDelayedMessages.insert(rlmsg);
        scheduleAt(simTime() + 180 + exponential(std::max(copyLifetime - 180, 0)), rlmsg);
      } else if (u->ownedKeys.find(msg->getKey()) == u->ownedKeys.end()) {
        NetInfAPIAddLocatorCall *alcall = new NetInfAPIAddLocatorCall();
        alcall->setKey(msg->getKey());
        alcall->setOwnerString(context->entry->ownerString.c_str());
        alcall->setLocatorUser(context->userIdx);
        alcall->setVersion(version);
        sendInternalRpcCall(TIER1_COMP, alcall,
            new StatsContext(globalStatistics->isMeasuring(),
                context->entry, version, context->userIdx));

        NetInfStorTestRemoveLocatorMessage *rlmsg = new NetInfStorTestRemoveLocatorMessage("remove_locator");
        rlmsg->setKey(msg->getKey());
        rlmsg->setUserIdx(context->userIdx);
        u->copiedKeys.insert(std::make_pair(msg->getKey(), std::make_pair(version, rlmsg)));
        pendingDelayedMessages.insert(rlmsg);
        scheduleAt(simTime() + 180 + exponential(std::max(copyLifetime - 180, 0)), rlmsg);
      }
    }

    if (u->pendingClientRequests == -1) {
      // Start client requests
      u->pendingClientRequests = 0;
      std::vector<OverlayKey>::const_iterator liter;
      for (liter = context->entry->linkedEntries.begin(); liter != context->entry->linkedEntries.end(); liter++) {
        NetInfGlobalDataStorage::Entry *e = globalDataStorage->getEntry(*liter);

        NetInfStorTestDelayMessage *dmsg = new NetInfStorTestDelayMessage("delay_message");
        dmsg->setKey(e->key);
        dmsg->setUserIdx(context->userIdx);

        u->pendingClientRequests++;

        // A WEB BROWSING WORKLOAD MODEL FOR SIMULATION
        //   Lourens O. Walters
        // Table 6.14
        // Web Client request inter-arrivel time

        int usDelay = std::min(std::max(lognormal(11.171529, 2.928586) + 78, 1.0), 299899922.0);
        pendingDelayedMessages.insert(dmsg);
        scheduleAt(simTime() + usDelay/1000000.0, dmsg);
      }

      if (u->pendingClientRequests == 0)
        handleUserGet(u->idx);
    } else {
      // A client request
      handleUserGet(u->idx);
    }
  }
  delete context;
}

void NetInfStorTest::handleTimerEvent(cMessage* msg)
{
  if (msg->isName("preseed_timer")) {
    // retry in 5 seconds if the network is still in initialization phase
    if (underlayConfigurator->isInInitPhase()) {
      scheduleAt(simTime() + 5, msg);
      return;
    } else if (underlayConfigurator->isSimulationEndingSoon() || nodeIsLeavingSoon) {
      return;
    }

    startUsersPut();
  } else if (msg->isName("delay_message")) {
    NetInfStorTestDelayMessage *dmsg = dynamic_cast<NetInfStorTestDelayMessage*>(msg);

    pendingDelayedMessages.erase(dmsg);

    NetInfGlobalDataStorage::Entry *e = globalDataStorage->getEntry(dmsg->getKey());
    if (e) {
      NetInfAPIGetCall* gcall = new NetInfAPIGetCall();
      gcall->setKey(e->key);
      gcall->setOriginatorUser(dmsg->getUserIdx());

      RECORD_STATS(numSent++; numGetSent++);
        
      sendInternalRpcCall(TIER1_COMP, gcall, new StatsContext(globalStatistics->isMeasuring(),
          e, 0, dmsg->getUserIdx()));
    }
    cancelAndDelete(dmsg);
  } else if (msg->isName("user_message")) {
    NetInfStorTestUserMessage *umsg = dynamic_cast<NetInfStorTestUserMessage*>(msg);

    pendingDelayedMessages.erase(umsg);

    User *u = &users[umsg->getUserIdx()];

    if (umsg->getStart()) {
      if (globalDataStorage->isSeeding()) {
        pendingDelayedMessages.insert(umsg);
        scheduleAt(simTime() + 10, umsg);
      } else {
        cancelAndDelete(umsg);
        startUsersGet();
      }
    } else {
      if (underlayConfigurator->isSimulationEndingSoon()
          || nodeIsLeavingSoon) {
        cancelAndDelete(umsg);
        return;
      }

      NetInfGlobalDataStorage::Entry *e = globalDataStorage->getRandomEntryForGet(thisNode, stringName);

      if (e) {
        NetInfAPIGetCall* gcall = new NetInfAPIGetCall();
        gcall->setKey(e->key);
        gcall->setOriginatorUser(u->idx);
        RECORD_STATS(numSent++; numGetSent++);

        sendInternalRpcCall(TIER1_COMP, gcall,
            new StatsContext(globalStatistics->isMeasuring(),
              e, -1, u->idx));
      } else {
        handleUserGet(u->idx);
      }
      cancelAndDelete(umsg);
    }
  } else if (msg->isName("remove_locator")) {
    NetInfStorTestRemoveLocatorMessage *rlmsg = static_cast<NetInfStorTestRemoveLocatorMessage*>(msg);

    pendingDelayedMessages.erase(rlmsg);

    User* u = &users[rlmsg->getUserIdx()];
    u->copiedKeys.erase(rlmsg->getKey());

    NetInfGlobalDataStorage::Entry *e = globalDataStorage->getEntry(rlmsg->getKey());
    if (e) {
      NetInfAPIRemoveLocatorCall *rlcall = new NetInfAPIRemoveLocatorCall();
      rlcall->setKey(rlmsg->getKey());
      rlcall->setLocatorUser(u->idx);
      sendInternalRpcCall(TIER1_COMP, rlcall,
          new StatsContext(globalStatistics->isMeasuring(),
            e, -1, u->idx));
    }
    cancelAndDelete(rlmsg);
  } else if (msg->isName("update_entry")) {
    NetInfStorTestUpdateEntryMessage* uemsg = static_cast<NetInfStorTestUpdateEntryMessage*>(msg);

    pendingDelayedMessages.erase(uemsg);

    NetInfGlobalDataStorage::Entry *e = globalDataStorage->getEntry(uemsg->getKey());
    if (e) {
      NetInfAPIPutCall *pcall = new NetInfAPIPutCall();
      pcall->setKey(uemsg->getKey());
      pcall->setOwnerUser(uemsg->getUserIdx());
      pcall->setVersion(++e->version);

      sendInternalRpcCall(TIER1_COMP, pcall,
          new StatsContext(globalStatistics->isMeasuring(),
            e, e->version, uemsg->getUserIdx()));
    }
    cancelAndDelete(uemsg);
  } if (msg == statisticsTimer) {
    scheduleAt(simTime() + statisticsSampleInterval, statisticsTimer);

    if (enableTimeStatistics && stringName == sampleApp && sampledClientRequests.size() > 0) {
      UNORDERED_MAP<std::string, int>::const_iterator iter;
      int x = 0;
      for (iter = sampledClientRequests.begin(); iter != sampledClientRequests.end(); iter++, x++) {
        globalStatistics->recordOutVector("NetInfStorTest: Client Requests", iter->second);
      }
      for (; x < numNodes; x++)
        globalStatistics->recordOutVector("NetInfStorTest: Client Requests", 0);
      sampledClientRequests.clear();
    }
  }

}


void NetInfStorTest::handleNodeLeaveNotification()
{
    nodeIsLeavingSoon = true;
}

void NetInfStorTest::finishApp()
{
    runningApps--;

    UNORDERED_SET<cMessage*>::iterator iter;
    for (iter = pendingDelayedMessages.begin(); iter != pendingDelayedMessages.end(); iter++)
      cancelAndDelete(*iter);
    pendingDelayedMessages.clear();

    for (std::vector<User>::iterator iter = users.begin(); iter != users.end(); iter++) {
      iter->ownedKeys.clear();
      iter->copiedKeys.clear();
    }

    simtime_t time = globalStatistics->calcMeasuredLifetime(creationTime);

    if (time >= GlobalStatistics::MIN_MEASURED) {
        // record scalar data
        globalStatistics->addStdDev("NetInfStorTest: Sent Total Messages/s",
                                    numSent / time);
        globalStatistics->addStdDev("NetInfStorTest: Sent GET Messages/s",
                                    numGetSent / time);
        globalStatistics->addStdDev("NetInfStorTest: Failed GET Requests/s",
                                    numGetError / time);
        globalStatistics->addStdDev("NetInfStorTest: Successful GET Requests/s",
                                    numGetSuccess / time);

        globalStatistics->addStdDev("NetInfStorTest: Sent PUT Messages/s",
                                    numPutSent / time);
        globalStatistics->addStdDev("NetInfStorTest: Failed PUT Requests/s",
                                    numPutError / time);
        globalStatistics->addStdDev("NetInfStorTest: Successful PUT Requests/s",
                                    numPutSuccess / time);

        globalStatistics->addStdDev("NetInfStorTest: Sent modification PUT Messages/s",
                                    numModSent / time);
        globalStatistics->addStdDev("NetInfStorTest: Failed modification PUT Requests/s",
                                    numModError / time);
        globalStatistics->addStdDev("NetInfStorTest: Successful modification PUT Requests/s",
                                    numModSuccess / time);

        if ((numGetSuccess + numGetError) > 0) {
            globalStatistics->addStdDev("NetInfStorTest: GET Success Ratio",
                                        (double) numGetSuccess
                                        / (double) (numGetSuccess + numGetError));
        }
        if ((numPutSuccess + numPutError) > 0) {
            globalStatistics->addStdDev("NetInfStorTest: PUT Success Ratio",
                                        (double) numPutSuccess
                                        / (double) (numPutSuccess + numPutError));
        }
        if ((numModSuccess + numModError) > 0) {
            globalStatistics->addStdDev("NetInfStorTest: modification PUT Success Ratio",
                                        (double) numModSuccess
                                        / (double) (numModSuccess + numModError));
        }
    }

    if (runningApps == 0) {
      UNORDERED_MAP<std::string, int>::const_iterator iter;
      int x = 0;
      for (iter = clientRequests.begin(); iter != clientRequests.end(); iter++, x++) {
        globalStatistics->addStdDev("NetInfStorTest: Client Requests", iter->second);
        if (enableTimeStatistics)
          globalStatistics->recordHistogram("NetInfStorTest: Client Requests", iter->second);
      }
      for (; x < numNodes; x++) {
        globalStatistics->addStdDev("NetInfStorTest: Client Requests", 0);
        if (enableTimeStatistics)
          globalStatistics->recordHistogram("NetInfStorTest: Client Requests", 0);
      }

      clientRequests.clear();
    }
    
    cancelAndDelete(statisticsTimer);
}

