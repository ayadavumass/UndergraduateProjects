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
 * @file DHT.h
 * @author Gregoire Menuel, Ingmar Baumgart
 */

#ifndef __DHT_H_
#define __DHT_H_

#include <omnetpp.h>

#include <OverlayKey.h>
#include <SHA1.h>
#include <CommonMessages_m.h>

#include "DHTMessage_m.h"
#include "DHTDataStorage.h"

#include "BaseApp.h"
#include <RpcMacros.h>

/**
 * A Distributed Hash Table (DHT) for KBR protocols
 *
 * A Distributed Hash Table (DHT) for KBR protocols
 */
class DHT : public BaseApp
{
public:
    DHT();
    virtual ~DHT();

protected:
    typedef std::vector<NodeHandle> ReplicaVector;

    struct GetMapEntry
    {
        ReplicaVector replica;
        std::map<BinaryValue, ReplicaVector> hashes;
        int numSent;
        int numAvailableReplica;
        int numResponses;
        DHTgetCAPICall* callMsg;
        ReplicaVector* hashVector;
    };

    struct PutMapEntry
    {
        int numSent;
        int numFailed;
        int numResponses;
        DHTputCAPICall* callMsg;
    };

    void initializeApp(int stage);
    void finishApp();
    void handleTimerEvent(cMessage* msg);

    bool handleRpcCall(BaseCallMessage* msg);
    void handleRpcResponse(BaseResponseMessage* msg, cPolymorphic *context,
                           int rpcId, simtime_t rtt);
    void handleRpcTimeout(BaseCallMessage* msg, const TransportAddress& dest,
                          cPolymorphic* context, int rpcId,
                          const OverlayKey& destKey);
    void handlePutRequest(DHTPutCall* dhtMsg);
    void handleGetRequest(DHTGetCall* dhtMsg);
    void handlePutResponse(DHTPutResponse* dhtMsg, int rpcId);
    void handleGetResponse(DHTGetResponse* dhtMsg, int rpcId);
    void handlePutCAPIRequest(DHTputCAPICall* capiPutMsg);
    void handleGetCAPIRequest(DHTgetCAPICall* capiPutMsg);
    void handleDumpDhtRequest(DHTdumpCall* call);
    void update(const NodeHandle& node, bool joined);
    void handleLookupResponse(LookupResponse* lookupMsg);


    int numReplica;
    int numGetRequests;
    double ratioIdentical;
    double maintenanceMessages;
    double normalMessages;
    double numBytesMaintenance;
    double numBytesNormal;
    simtime_t lastGetCall;
    std::map<unsigned int, BaseCallMessage*> rpcIdMap; /**< List of the Rpc Ids of the messages sent following the reception of an rpc request (the second member) */
    std::map<int, GetMapEntry> getMap;
    std::map<int, PutMapEntry> putMap;
    // module references
    DHTDataStorage* dataStorage; /**< pointer to the dht data storage */

private:
    int resultValuesBitLength(DHTGetResponse* msg);
};

#endif
