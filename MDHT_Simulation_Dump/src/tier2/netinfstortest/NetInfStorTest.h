//
// Copyright (C) 2007 Institut fuer Telematik, Universitaet Karlsruhe (TH)
// Copyright (C) 2010 Sebastian Dröge <slomo@upb.de>
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

#ifndef __NETINFSTORTEST_H__
#define __NETINFSTORTEST_H__

#include <omnetpp.h>

#include <GlobalNodeList.h>
#include <GlobalStatistics.h>
#include <UnderlayConfigurator.h>
#include <TransportAddress.h>
#include <OverlayKey.h>
#include <InitStages.h>
#include <BinaryValue.h>
#include <MDHTBaseApp.h>
#include <set>
#include <sstream>

#include <HostnameInfo.h>
#include "../../applications/mDHT/mDHT_m.h"

#include "NetInfGlobalDataStorage.h"

class NetInfStorTest : public MDHTBaseApp
{
private:
    class StatsContext : public cPolymorphic
    {
    public:
        bool measurementPhase;

        const NetInfGlobalDataStorage::Entry* entry;
        
        int version;

        int userIdx;

        StatsContext(bool measurementPhase,
                     const NetInfGlobalDataStorage::Entry* entry,
                     int version,
                     int userIdx) :
            measurementPhase(measurementPhase),
            entry(entry),
            version(version),
            userIdx(userIdx) {};
    };

    void initializeApp(int stage);
    void finishApp();

    void handleGetResponse(NetInfAPIGetResponse* msg,
                                   StatsContext* context,
                                   simtime_t rtt);
    void handlePutResponse(NetInfAPIPutResponse* msg,
                                   StatsContext* context,
                                   simtime_t rtt);
    void handleAddLocatorResponse(NetInfAPIAddLocatorResponse* msg,
                                   StatsContext* context,
                                   simtime_t rtt);
    void handleRemoveLocatorResponse(NetInfAPIRemoveLocatorResponse* msg,
                                   StatsContext* context,
                                   simtime_t rtt);

    /**
     * processes self-messages
     *
     * method to handle self-messages
     * should be overwritten in derived application if needed
     * @param msg self-message
     */
    virtual void handleTimerEvent(cMessage* msg);

    virtual void handleNodeLeaveNotification();

    // see RpcListener.h
    void handleRpcResponse(BaseResponseMessage* msg, const RpcState& state,
                           simtime_t rtt);
    void handleRpcTimeout(BaseCallMessage* msg,                     // called when an RPC times out
                          const TransportAddress& dest,
                          cPolymorphic* context, int rpcId,
                          const OverlayKey&);

    UnderlayConfigurator* underlayConfigurator; /**< pointer to UnderlayConfigurator in this node */

    GlobalNodeList* globalNodeList; /**< pointer to GlobalNodeList in this node*/

    GlobalStatistics* globalStatistics; /**< pointer to GlobalStatistics module in this node*/
    NetInfGlobalDataStorage* globalDataStorage; /**< pointer to the NetInfGlobalDataStorage module */
    HostnameInfo* hostnameInfo;

    // properties
    int versionLifetime;
    int copyLifetime;
    double publishUserRatio;
    double publishCopyUserRatio;
    int statisticsSampleInterval;
    bool enableTimeStatistics;

    // statistics
    int numSent; /**< number of sent packets*/
    int numGetSent; /**< number of get sent*/
    int numGetError; /**< number of false get responses*/
    int numGetSuccess; /**< number of false get responses*/
    int numPutSent; /**< number of put sent*/
    int numPutError; /**< number of error in put responses*/
    int numPutSuccess; /**< number of success in put responses*/
    int numModSent; /**< number of modifications sent*/
    int numModError; /**< number of errors modifications*/
    int numModSuccess; /**< number of success in modifications*/

    cMessage *preseed_timer;
    bool nodeIsLeavingSoon; //!< true if the node is going to be killed shortly

    struct User {
      int idx;
      int nUserRequests;
      int curUserRequest;
      int pendingClientRequests;

      bool publish;

      UNORDERED_SET<OverlayKey> ownedKeys;
      // key->(version, timeout message)
      UNORDERED_MAP<OverlayKey, std::pair<int, cMessage*> > copiedKeys;

      User() : idx(-1), nUserRequests(-1), curUserRequest(-1), pendingClientRequests(-1), publish(false) {}
    };
    std::vector<User> users;

    void handleUserGet(int userIdx);
    void nextSession(int userIdx, bool first=false);
    void nextRequest(int userIdx);
    void startUsersPut();
    void startUsersGet();
    void nextPut(int userIdx);

    UNORDERED_SET<cMessage*> pendingDelayedMessages;

    std::string stringName;

    cMessage *statisticsTimer;

public:
    NetInfStorTest();

    /**
     * virtual destructor
     */
    virtual ~NetInfStorTest();
};

#endif
