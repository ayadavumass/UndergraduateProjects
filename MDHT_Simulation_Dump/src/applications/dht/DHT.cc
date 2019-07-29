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
 * @file DHT.cc
 * @author Gregoire Menuel, Ingmar Baumgart
 */

#include <IPAddressResolver.h>

#include "DHT.h"

#include <RpcMacros.h>
#include <BaseRpc.h>
#include <GlobalStatistics.h>

Define_Module(DHT);

using namespace std;

DHT::DHT()
{
    dataStorage = NULL;
}

DHT::~DHT()
{
    std::map<unsigned int, BaseCallMessage*>::iterator it;

    for (it = rpcIdMap.begin(); it != rpcIdMap.end(); it++) {
        cancelAndDelete(it->second);
        it->second = NULL;
    }

    std::map<int, GetMapEntry>::iterator it2;

    for (it2 = getMap.begin(); it2 != getMap.end(); it2++) {
        cancelAndDelete(it2->second.callMsg);
        it2->second.callMsg = NULL;
    }

    std::map<int, PutMapEntry>::iterator it3;

    for (it3 = putMap.begin(); it3 != putMap.end(); it3++) {
        cancelAndDelete(it3->second.callMsg);
        it3->second.callMsg = NULL;
    }

    rpcIdMap.clear();
    getMap.clear();
    putMap.clear();

    if (dataStorage != NULL) {
        dataStorage->clear();
    }
}

void DHT::initializeApp(int stage)
{
    if (stage != MIN_STAGE_APP)
        return;

    dataStorage = check_and_cast<DHTDataStorage*>
                      (getParentModule()->getSubmodule("dhtDataStorage"));

    numReplica = par("numReplica");
    numGetRequests = par("numGetRequests");
    ratioIdentical = par("ratioIdentical");

    if (numReplica > overlay->getMaxNumSiblings()) {
        opp_error("DHT::initialize(): numReplica bigger than what this "
                  "overlay can handle (%d)", overlay->getMaxNumSiblings());
    }

    maintenanceMessages = 0;
    normalMessages = 0;
    numBytesMaintenance = 0;
    numBytesNormal = 0;
    WATCH(maintenanceMessages);
    WATCH(normalMessages);
    WATCH(numBytesNormal);
    WATCH(numBytesMaintenance);
    WATCH_MAP(rpcIdMap);
}

void DHT::handleTimerEvent(cMessage* msg)
{
    DHTTtlTimer* msg_timer = dynamic_cast<DHTTtlTimer*> (msg);

    if (msg_timer) {
        EV << "[DHT::handleTimerEvent()]\n"
           << "    received timer ttl, key: "
           << msg_timer->getKey().toString(16)
           << "\n (overlay->getThisNode().getKey() = "
           << overlay->getThisNode().getKey().toString(16) << ")"
           << endl;

        dataStorage->removeData(msg_timer->getKey(), msg_timer->getKind(),
                                msg_timer->getId());
    }
}

bool DHT::handleRpcCall(BaseCallMessage* msg)
{
    // delegate messages
    RPC_SWITCH_START( msg )
        // RPC_DELEGATE( <messageName>[Call|Response], <methodToCall> )
        RPC_DELEGATE( DHTPut, handlePutRequest );
        RPC_DELEGATE( DHTGet, handleGetRequest );
        RPC_DELEGATE( DHTputCAPI, handlePutCAPIRequest ); //requests coming from an upper tier
        RPC_DELEGATE( DHTgetCAPI, handleGetCAPIRequest );
        RPC_DELEGATE( DHTdump, handleDumpDhtRequest );
    RPC_SWITCH_END( )

    return RPC_HANDLED;
}

void DHT::handleRpcResponse(BaseResponseMessage* msg, cPolymorphic* context,
                            int rpcId, simtime_t rtt)
{
    RPC_SWITCH_START(msg)
        RPC_ON_RESPONSE(DHTPut){
        handlePutResponse(_DHTPutResponse, rpcId);
        EV << "[DHT::handleRpcResponse()]\n"
           << "    DHT Put RPC Response received: id=" << rpcId
           << " msg=" << *_DHTPutResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_ON_RESPONSE(DHTGet) {
        handleGetResponse(_DHTGetResponse, rpcId);
        EV << "[DHT::handleRpcResponse()]\n"
           << "    DHT Get RPC Response received: id=" << rpcId
           << " msg=" << *_DHTGetResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_ON_RESPONSE(Lookup) {
        handleLookupResponse(_LookupResponse);
        EV << "[DHT::handleRpcResponse()]\n"
           << "    Replica Set RPC Response received: id=" << rpcId
           << " msg=" << *_LookupResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_SWITCH_END()
}

void DHT::handleRpcTimeout(BaseCallMessage* msg, const TransportAddress& dest,
                           cPolymorphic* context, int rpcId,
                           const OverlayKey& destKey)
{

    RPC_SWITCH_START(msg)
    RPC_ON_CALL(DHTPut){
        EV << "[DHT::handleRpcResponse()]\n"
           << "    DHTPut Timeout"
           << endl;

        std::map<int, PutMapEntry>::iterator it2 =
                putMap.find(rpcId);

        if (it2 == putMap.end()) //unknown request
            return;

        it2->second.numFailed++;

        if (it2->second.numFailed / (double)it2->second.numSent >= 0.5) {
            DHTputCAPIResponse* capiPutRespMsg = new DHTputCAPIResponse();
            capiPutRespMsg->setKey(_DHTPutCall->getKey());
            capiPutRespMsg->setIsSuccess(false);
            sendRpcResponse(it2->second.callMsg, capiPutRespMsg);
            //cout << "timeout 1" << endl;
            it2->second.callMsg = NULL;
            putMap.erase(rpcId);
        }

        break;
    }
    RPC_ON_CALL(DHTGet) {
        EV << "[DHT::handleRpcResponse()]\n"
           << "    DHTGet Timeout"
           << endl;

        std::map<unsigned int, BaseCallMessage*>::iterator it =
            rpcIdMap.find(_DHTGetCall->getNonce());
        std::map<int, GetMapEntry>::iterator it2 =
            getMap.find(rpcId);

        if (it2 == getMap.end()) { //unknown request
            return;
        }

        if (it != rpcIdMap.end()) {
            //we have sent a 'real' get request
            rpcIdMap.erase(_DHTGetCall->getNonce());
            //ask anyone else, if possible

            if ((it2->second.hashVector != NULL)
                && (it2->second.hashVector->size()> 0)) {

                DHTGetCall* getCall = new DHTGetCall();
                getCall->setKey(_DHTGetCall->getKey());
                getCall->setKind(_DHTGetCall->getKind());
                getCall->setId(_DHTGetCall->getId());
                getCall->setIsHash(false);
                getCall->setBitLength(GETCALL_L(getCall));
                RECORD_STATS(normalMessages++;
                             numBytesNormal += getCall->getByteLength());

                int nonce = sendRouteRpcCall(TIER1_COMP,
                                             it2->second.hashVector->back(),
                                             getCall, NULL, DEFAULT_ROUTING,
                                             -1, 0, rpcId);
                rpcIdMap.insert(make_pair(nonce, (BaseCallMessage*)NULL));
                it2->second.hashVector->pop_back();
            } else {
                // no one else
                DHTgetCAPIResponse* capiGetRespMsg = new DHTgetCAPIResponse();
                capiGetRespMsg->setIsSuccess(false);
                sendRpcResponse(it2->second.callMsg,
                                capiGetRespMsg);
                //cout << "DHT: GET failed: no one else" << endl;
                getMap.erase(rpcId);
                return;
            }

        } else {
            //try to ask another one of the replica list for the hash
            if (it2->second.replica.size() > 0) {
                DHTGetCall* getCall = new DHTGetCall();
                getCall->setKey(_DHTGetCall->getKey());
                getCall->setKind(_DHTGetCall->getKind());
                getCall->setId(_DHTGetCall->getId());
                getCall->setIsHash(true);
                getCall->setBitLength(GETCALL_L(getCall));

                RECORD_STATS(normalMessages++;
                             numBytesNormal += getCall->getByteLength());

                sendRouteRpcCall(TIER1_COMP, it2->second.replica.back(),
                                 getCall, NULL, DEFAULT_ROUTING, -1, 0,
                                 rpcId);
                it2->second.replica.pop_back();
            } else {
                // no one else to ask, see what we can do with what we have
                if (it2->second.numResponses > 0) {
                    unsigned int maxCount = 0;
                    ReplicaVector* hashVector = NULL;
                    std::map<BinaryValue, ReplicaVector>::iterator itHashes;
                    for (itHashes = it2->second.hashes.begin();
                         itHashes != it2->second.hashes.end(); itHashes++) {

                        if (itHashes->second.size()> maxCount) {
                            maxCount = itHashes->second.size();
                            hashVector = &(itHashes->second);
                        }
                    }

                    if ((double)maxCount/(double)it2->second.numResponses >=
                                                             ratioIdentical) {
                        it2->second.hashVector = hashVector;
                    }
                }

                if ((it2->second.hashVector != NULL)
                     && (it2->second.hashVector->size()> 0)) {

                    DHTGetCall* getCall = new DHTGetCall();
                    getCall->setKey(_DHTGetCall->getKey());
                    getCall->setKind(_DHTGetCall->getKind());
                    getCall->setId(_DHTGetCall->getId());
                    getCall->setIsHash(false);
                    getCall->setBitLength(GETCALL_L(getCall));
                    RECORD_STATS(normalMessages++;
                                 numBytesNormal += getCall->getByteLength());
                    int nonce = sendRouteRpcCall(TIER1_COMP,
                                                 it2->second.hashVector->back(),
                                                 getCall, NULL, DEFAULT_ROUTING,
                                                 -1, 0, rpcId);
                    rpcIdMap.insert(make_pair(nonce, (BaseCallMessage*)NULL));
                    it2->second.hashVector->pop_back();
                } else {
                    DHTgetCAPIResponse* capiGetRespMsg = new DHTgetCAPIResponse();
                    capiGetRespMsg->setIsSuccess(false);
                    sendRpcResponse(it2->second.callMsg, capiGetRespMsg);
                    //cout << "DHT: GET failed: timeout (no one else)" << endl;
                    getMap.erase(rpcId);
                }
            }
        }
        break;
    }
    RPC_SWITCH_END( )
}

void DHT::handlePutRequest(DHTPutCall* dhtMsg)
{
    std::string tempString = "PUT_REQUEST received: "
            + std::string(dhtMsg->getKey().toString(16));
    getParentModule()->getParentModule()->bubble(tempString.c_str());

    if (!(dataStorage->isModifiable(dhtMsg->getKey(), dhtMsg->getKind(),
                                    dhtMsg->getId()))) {
        // check if the put request came from the right node
        NodeHandle sourceNode = dataStorage->getSourceNode(dhtMsg->getKey(),
                                    dhtMsg->getKind(), dhtMsg->getId());
        if (((!sourceNode.isUnspecified())
                && (!dhtMsg->getSrcNode().isUnspecified()) && (sourceNode
                != dhtMsg->getSrcNode())) || ((dhtMsg->getMaintenance())
                && (dhtMsg->getOwnerNode() == sourceNode))) {
            // TODO: set owner
            DHTPutResponse* responseMsg = new DHTPutResponse();
            responseMsg->setKey(dhtMsg->getKey());
            tempString = "Error, not allowed to modify this key";
            responseMsg->setValue(BinaryValue(tempString));
            responseMsg->setBitLength(PUTRESPONSE_L(responseMsg));
            RECORD_STATS(normalMessages++;
                         numBytesNormal += responseMsg->getByteLength());
            sendRpcResponse(dhtMsg, responseMsg);
            return;
        }

    }

    // remove data item from local data storage
    dataStorage->removeData(dhtMsg->getKey(), dhtMsg->getKind(),
                            dhtMsg->getId());

    if (dhtMsg->getValue().size() > 0) {
        // add ttl timer
        DHTTtlTimer *timerMsg = NULL;
        if (dhtMsg->getTtl() > 0) {
          timerMsg = new DHTTtlTimer("ttl_timer");
          timerMsg->setKey(dhtMsg->getKey());
          timerMsg->setKind(dhtMsg->getKind());
          timerMsg->setId(dhtMsg->getId());
          scheduleAt(simTime() + dhtMsg->getTtl(), timerMsg);
        }
        // storage data item in local data storage
        bool err;
        dataStorage->addData(dhtMsg->getKey(), dhtMsg->getKind(),
        		             dhtMsg->getId(), dhtMsg->getValue(), timerMsg,
                             dhtMsg->getIsModifiable(), dhtMsg->getSrcNode(),
                             overlay->isSiblingFor(overlay->getThisNode(),
                                                   dhtMsg->getKey(),
                                                   1, &err));
    }

    // send back
    DHTPutResponse* responseMsg = new DHTPutResponse();
    responseMsg->setKey(dhtMsg->getKey());
    responseMsg->setValue(dhtMsg->getValue());
    responseMsg->setBitLength(PUTRESPONSE_L(responseMsg));
    RECORD_STATS(normalMessages++; numBytesNormal += responseMsg->getByteLength());

    sendRpcResponse(dhtMsg, responseMsg);
}

void DHT::handleGetRequest(DHTGetCall* dhtMsg)
{
	EV << "[DHT::handleGetRequest()] method called" << endl;
    std::string tempString = "GET_REQUEST received: "
            + std::string(dhtMsg->getKey().toString(16));

    getParentModule()->getParentModule()->bubble(tempString.c_str());

    if (dhtMsg->getKey().isUnspecified()) {
        throw cRuntimeError("DHT::handleGetRequest: Unspecified key!");
    }

    // send back
    DHTGetResponse* responseMsg = new DHTGetResponse();
    responseMsg->setKey(dhtMsg->getKey());
    responseMsg->setIsHash(dhtMsg->getIsHash());

    DhtDumpVector* dataVect = dataStorage->dumpDht(dhtMsg->getKey(),
        dhtMsg->getKind(),
        dhtMsg->getId());

    if (dataVect->size() == 0) {
        responseMsg->setHashValue(BinaryValue::UNSPECIFIED_VALUE);
        responseMsg->setResultArraySize(0);
    } else {
        if (dhtMsg->getIsHash()) {
            // TODO: verify this
            BinaryValue resultValues;
            for (uint32_t i = 0; i < dataVect->size(); i++) {
                resultValues += (*dataVect)[i].getValue();
            }

            CSHA1 sha1;
            BinaryValue hashValue(20);
            sha1.Reset();
            sha1.Update((uint8_t*) (&(*resultValues.begin())),
                        resultValues.size());
            sha1.Final();
            sha1.GetHash((unsigned char*)&hashValue[0]);

            responseMsg->setHashValue(hashValue);
        } else {
            responseMsg->setResultArraySize(dataVect->size());

            for (uint32_t i = 0; i < dataVect->size(); i++) {
                responseMsg->setResult(i, (*dataVect)[i]);
            }

        }
    }
    delete dataVect;

    responseMsg->setBitLength(GETRESPONSE_L(responseMsg));
    RECORD_STATS(normalMessages++;
                 numBytesNormal += responseMsg->getByteLength());
    EV << "    sending response (" << responseMsg->getResultArraySize() << ")" << endl;
    sendRpcResponse(dhtMsg, responseMsg);
}

void DHT::handlePutCAPIRequest(DHTputCAPICall* capiPutMsg)
{
    //asks the replica list
    LookupCall* replicaMsg = new LookupCall();
    replicaMsg->setKey(capiPutMsg->getKey());
    replicaMsg->setNumSiblings(numReplica);
    int nonce = sendInternalRpcCall(OVERLAY_COMP, replicaMsg);
    rpcIdMap.insert(make_pair(nonce, capiPutMsg));
}

void DHT::handleGetCAPIRequest(DHTgetCAPICall* capiGetMsg)
{
    LookupCall* replicaMsg = new LookupCall();
    replicaMsg->setKey(capiGetMsg->getKey());
    replicaMsg->setNumSiblings(numReplica);
    int nonce = sendInternalRpcCall(OVERLAY_COMP, replicaMsg);
    rpcIdMap.insert(make_pair(nonce, capiGetMsg));
    lastGetCall = simTime();
}

void DHT::handleDumpDhtRequest(DHTdumpCall* call)
{
    DHTdumpResponse* response = new DHTdumpResponse();
    DhtDumpVector* dumpVector = dataStorage->dumpDht();

    response->setRecordArraySize(dumpVector->size());

    for (uint32_t i = 0; i < dumpVector->size(); i++) {
        response->setRecord(i, (*dumpVector)[i]);
    }

    delete dumpVector;

    sendRpcResponse(call, response);
}

void DHT::handlePutResponse(DHTPutResponse* dhtMsg, int rpcId)
{
    std::map<int, PutMapEntry>::iterator it2 =
            putMap.find(rpcId);

    if (it2 == putMap.end()) //unknown request
        return;

    it2->second.numResponses++;

    if (it2->second.numResponses / (double)it2->second.numSent > 0.5) {
        DHTputCAPIResponse* capiPutRespMsg = new DHTputCAPIResponse();
        capiPutRespMsg->setKey(dhtMsg->getKey());
        capiPutRespMsg->setValue(dhtMsg->getValue());
        capiPutRespMsg->setIsSuccess(true);
        sendRpcResponse(it2->second.callMsg, capiPutRespMsg);
        it2->second.callMsg = NULL;
        putMap.erase(rpcId);
    }
}

void DHT::handleGetResponse(DHTGetResponse* dhtMsg, int rpcId)
{
    std::map<unsigned int, BaseCallMessage*>::iterator it =
            rpcIdMap.find(dhtMsg->getNonce());
    std::map<int, GetMapEntry>::iterator it2 =
            getMap.find(rpcId);

    if (it2 == getMap.end()) //unknown request
        return;

    if (it != rpcIdMap.end()) {
        //we have sent a 'real' get request
        rpcIdMap.erase(dhtMsg->getNonce());
        if (!dhtMsg->getIsHash()) {
            // TODO verify hash
            DHTgetCAPIResponse* capiGetRespMsg = new DHTgetCAPIResponse();
            capiGetRespMsg->setResultArraySize(dhtMsg->getResultArraySize());
            for (uint i = 0; i < dhtMsg->getResultArraySize(); i++) {
                capiGetRespMsg->setResult(i, dhtMsg->getResult(i));
            }
            capiGetRespMsg->setIsSuccess(true);
            sendRpcResponse(it2->second.callMsg, capiGetRespMsg);
            getMap.erase(rpcId);
            return;
        }
    } else {
        if (dhtMsg->getIsHash()) {
            std::map<BinaryValue, ReplicaVector>::iterator itHashes =
                    it2->second.hashes.find(dhtMsg->getHashValue());

            if (itHashes == it2->second.hashes.end()) {
                //new hash
                ReplicaVector vect = ReplicaVector();
                vect.push_back(dhtMsg->getSrcNode());
                it2->second.hashes.insert(make_pair(dhtMsg->getHashValue(),
                                                    vect));
            } else {
                itHashes->second.push_back(dhtMsg->getSrcNode());
            }

            it2->second.numResponses++;

            //we've got all the answers we wanted
            unsigned int maxCount = 0;
            ReplicaVector* hashVector = NULL;
            for (itHashes = it2->second.hashes.begin(); itHashes
            != it2->second.hashes.end(); itHashes++) {
                if (itHashes->second.size() > maxCount) {
                    maxCount = itHashes->second.size();
                    hashVector = &(itHashes->second);
                }
            }

            if ((double) maxCount / (double) it2->second.numAvailableReplica
                    >= ratioIdentical) {
                it2->second.hashVector = hashVector;
            } else if (it2->second.numResponses >= numGetRequests){
                //we'll try to ask some other nodes
                if (it2->second.replica.size() > 0) {
                    DHTGetCall* getCall = new DHTGetCall();
                    getCall->setKey(dhtMsg->getKey());
                    getCall->setKind(dhtMsg->getKind());
                    getCall->setId(dhtMsg->getId());
                    getCall->setIsHash(true);
                    getCall->setBitLength(GETCALL_L(getCall));
                    RECORD_STATS(normalMessages++;
                                     numBytesNormal += getCall->getByteLength());
                    sendRouteRpcCall(TIER1_COMP,
                                     it2->second.replica.back(), getCall,
                                     NULL, DEFAULT_ROUTING, -1, 0,
                                     it2->second.callMsg->getNonce());
                    it2->second.replica.pop_back();
                } else { //we don't have anyone else to ask
                    DHTgetCAPIResponse* capiGetRespMsg =
                        new DHTgetCAPIResponse();
                    DhtDumpEntry result;
                    result.setKey(dhtMsg->getKey());
                    result.setValue(BinaryValue::UNSPECIFIED_VALUE);
                    capiGetRespMsg->setResultArraySize(1);
                    capiGetRespMsg->setResult(0, result);
                    capiGetRespMsg->setIsSuccess(false);
                    sendRpcResponse(it2->second.callMsg, capiGetRespMsg);
                    //cout << "DHT: GET failed: hash (no one else)" << endl;
                    getMap.erase(rpcId);
                    return;
                }

            }
        }
    }

    if (it2->second.hashVector != NULL) {
        //we have already received all the response and chosen a hash
        if (it2->second.hashVector->size() > 0) {
            DHTGetCall* getCall = new DHTGetCall();
            getCall->setKey(it2->second.callMsg->getKey());
            getCall->setKind(it2->second.callMsg->getKind());
            getCall->setId(it2->second.callMsg->getId());
            getCall->setIsHash(false);
            getCall->setBitLength(GETCALL_L(getCall));
            RECORD_STATS(normalMessages++;
                         numBytesNormal += getCall->getByteLength());
            int nonce = sendRouteRpcCall(TIER1_COMP,
                                         it2->second.hashVector->back(),
                                         getCall, NULL, DEFAULT_ROUTING,
                                         -1, 0, it2->second.callMsg->getNonce());
            rpcIdMap.insert(make_pair(nonce, (BaseCallMessage*)NULL));
            it2->second.hashVector->pop_back();
        } else { //we don't have anyone else to ask
            DHTgetCAPIResponse* capiGetRespMsg = new DHTgetCAPIResponse();
            DhtDumpEntry result;
            result.setKey(dhtMsg->getKey());
            result.setValue(BinaryValue::UNSPECIFIED_VALUE);
            capiGetRespMsg->setResultArraySize(1);
            capiGetRespMsg->setResult(0, result);
            capiGetRespMsg->setIsSuccess(false);
            sendRpcResponse(it2->second.callMsg, capiGetRespMsg);
            //cout << "DHT: GET failed: hash2 (no one else)" << endl;
            getMap.erase(rpcId);
        }
    }
}

void DHT::update(const NodeHandle& node, bool joined)
{
    OverlayKey key;
    DHTPutCall* dhtMsg;
    bool err = false;
    DhtDataEntry entry;
    std::map<OverlayKey, DhtDataEntry>::iterator it = dataStorage->begin();
    for (unsigned int i = 0; i < dataStorage->getSize(); i++) {
        key = it->first;
        entry = it->second;
        if (joined) {
            if (entry.responsible && (overlay->isSiblingFor(node, key,
                                                            numReplica, &err)
                    || err)) { // hack for Chord, if we've got a new predecessor

                dhtMsg = new DHTPutCall();
                dhtMsg->setKey(key);
                dhtMsg->setKind(entry.kind);
                dhtMsg->setId(entry.id);
                dhtMsg->setValue(entry.value);
                dhtMsg->setTtl((entry.ttlMessage ? (int)SIMTIME_DBL(entry.ttlMessage->getArrivalTime()
                        - simTime()) : 0));
                dhtMsg->setIsModifiable(entry.is_modifiable);
                dhtMsg->setMaintenance(true);
                dhtMsg->setBitLength(PUTCALL_L(dhtMsg));
                RECORD_STATS(maintenanceMessages++;
                        numBytesMaintenance += dhtMsg->getByteLength());
                sendRouteRpcCall(TIER1_COMP, node, dhtMsg);
            }

            if (err) {
                EV << "[DHT::update()]\n"
                   << "    Unable to know if key: " << key
                   << " is in range of node: " << node
                   << endl;
            }
        } else {
#if 0
            //the update concerns a node who has left
            //replicate
            LookupCall* replicaMsg = new LookupCall();
            replicaMsg->setKey(key);
            replicaMsg->setNumSiblings(numReplica);
            int nonce = sendInternalRpcCall(OVERLAY_COMP,
                                            replicaMsg);
            dhtMsg = new DHTPutCall();
            dhtMsg->setKey(key);
            dhtMsg->setValue(entry.value);
            dhtMsg->setTtl((int)(entry.ttlMessage->getArrivalTime()
                    - simTime()));
            dhtMsg->setIsModifiable(entry.is_modifiable);
            dhtMsg->setMaintenance(true);
            dhtMsg->setBitLength(PUTCALL_L(dhtMsg));

            rpcIdMap.insert(make_pair(nonce, dhtMsg));
#endif
        }

        entry.responsible = overlay->isSiblingFor(overlay->getThisNode(),
                                                  key, 1, &err);
        it++;
    }
}

void DHT::handleLookupResponse(LookupResponse* lookupMsg)
{
    std::map<unsigned int, BaseCallMessage*>::iterator it =
            rpcIdMap.find(lookupMsg->getNonce());

    if (it == rpcIdMap.end() || it->second == NULL)
        return;

    if (dynamic_cast<DHTputCAPICall*> (it->second)) {

        #if 0
        cout << "DHT::handleLookupResponse(): PUT "
             << lookupMsg->getKey() << " ("
             << overlay->getThisNode().getKey() << ")" << endl;

        for (unsigned int i = 0; i < lookupMsg->getSiblingsArraySize(); i++) {
            cout << i << ": " << lookupMsg->getSiblings(i) << endl;
        }
#endif

        DHTputCAPICall* capiPutMsg = dynamic_cast<DHTputCAPICall*> (it->second);
        rpcIdMap.erase(lookupMsg->getNonce());

        if ((lookupMsg->getIsValid() == false)
                || (lookupMsg->getSiblingsArraySize() == 0)) {

            EV << "[DHT::handleLookupResponse()]\n"
               << "    Unable to get replica list : invalid lookup"
               << endl;
            DHTputCAPIResponse* capiPutRespMsg = new DHTputCAPIResponse();
            capiPutRespMsg->setKey(lookupMsg->getKey());
            capiPutRespMsg->setIsSuccess(false);
            //cout << "DHT::lookup failed" << endl;
            sendRpcResponse(capiPutMsg, capiPutRespMsg);
            return;
        }

        if ((capiPutMsg->getId() == 0) && (capiPutMsg->getValue().size() > 0)) {
            // pick a random id before replication of the data item
            // id 0 is kept for delete requests (i.e. size = 0)
            capiPutMsg->setId(intuniform(1, 2147483647));
        }

        for (unsigned int i = 0; i < lookupMsg->getSiblingsArraySize(); i++) {
            DHTPutCall* dhtMsg = new DHTPutCall();
            dhtMsg->setKey(capiPutMsg->getKey());
            dhtMsg->setKind(capiPutMsg->getKind());
            dhtMsg->setId(capiPutMsg->getId());
            dhtMsg->setValue(capiPutMsg->getValue());
            dhtMsg->setTtl(capiPutMsg->getTtl());
            dhtMsg->setIsModifiable(capiPutMsg->getIsModifiable());
            dhtMsg->setMaintenance(false);
            dhtMsg->setBitLength(PUTCALL_L(dhtMsg));
            RECORD_STATS(normalMessages++;
                         numBytesNormal += dhtMsg->getByteLength());
            sendRouteRpcCall(TIER1_COMP, lookupMsg->getSiblings(i),
                             dhtMsg, NULL, DEFAULT_ROUTING, -1,
                             0, capiPutMsg->getNonce());
        }

        PutMapEntry mapEntry;
        mapEntry.callMsg = capiPutMsg;
        mapEntry.numResponses = 0;
        mapEntry.numFailed = 0;
        mapEntry.numSent = lookupMsg->getSiblingsArraySize();

        putMap.insert(make_pair(capiPutMsg->getNonce(), mapEntry));
    }
    else if (dynamic_cast<DHTgetCAPICall*>(it->second)) {

#if 0
        cout << "DHT::handleLookupResponse(): GET "
             << lookupMsg->getKey() << " ("
             << overlay->getThisNode().getKey() << ")" << endl;

        for (unsigned int i = 0; i < lookupMsg->getSiblingsArraySize(); i++) {
            cout << i << ": " << lookupMsg->getSiblings(i) << endl;
        }
#endif

        DHTgetCAPICall* capiGetMsg = dynamic_cast<DHTgetCAPICall*>(it->second);
        rpcIdMap.erase(lookupMsg->getNonce());

        if ((lookupMsg->getIsValid() == false)
                || (lookupMsg->getSiblingsArraySize() == 0)) {

            EV << "[DHT::handleLookupResponse()]\n"
               << "    Unable to get replica list : invalid lookup"
               << endl;
            DHTgetCAPIResponse* capiGetRespMsg = new DHTgetCAPIResponse();
            DhtDumpEntry result;
            result.setKey(lookupMsg->getKey());
            result.setValue(BinaryValue::UNSPECIFIED_VALUE);
            capiGetRespMsg->setResultArraySize(1);
            capiGetRespMsg->setResult(0, result);
            capiGetRespMsg->setIsSuccess(false);
            //cout << "DHT: lookup failed 2" << endl;
            sendRpcResponse(capiGetMsg, capiGetRespMsg);
            return;
        }

        GetMapEntry mapEntry;
        mapEntry.numSent = 0;

        for (unsigned int i = 0; i < lookupMsg->getSiblingsArraySize(); i++) {
            if (i < (unsigned int)numGetRequests) {
                DHTGetCall* dhtMsg = new DHTGetCall();
                dhtMsg->setKey(capiGetMsg->getKey());
                dhtMsg->setKind(capiGetMsg->getKind());
                dhtMsg->setId(capiGetMsg->getId());
                dhtMsg->setIsHash(true);
                dhtMsg->setBitLength(GETCALL_L(dhtMsg));
                RECORD_STATS(normalMessages++;
                             numBytesNormal += dhtMsg->getByteLength());
                sendRouteRpcCall(TIER1_COMP, lookupMsg->getSiblings(i), dhtMsg,
                                 NULL, DEFAULT_ROUTING, -1, 0,
                                 capiGetMsg->getNonce());
                mapEntry.numSent++;
            } else {
                //We don't send, we just store the remaining keys
                mapEntry.replica.push_back(lookupMsg->getSiblings(i));
            }
        }

        mapEntry.numAvailableReplica = lookupMsg->getSiblingsArraySize();
        mapEntry.callMsg = capiGetMsg;
        mapEntry.numResponses = 0;
        mapEntry.hashVector = NULL;

        getMap.insert(make_pair(capiGetMsg->getNonce(), mapEntry));
    }
    else if (dynamic_cast<DHTPutCall*>(it->second)) {
        DHTPutCall* putMsg = dynamic_cast<DHTPutCall*>(it->second);
        rpcIdMap.erase(lookupMsg->getNonce());

        if ((lookupMsg->getIsValid() == false)
                || (lookupMsg->getSiblingsArraySize() == 0)) {

            EV << "[DHT::handleLookupResponse()]\n"
               << "    Unable to get replica list : invalid lookup"
               << endl;
            delete putMsg;
            return;
        }

        for( unsigned int i = 0; i < lookupMsg->getSiblingsArraySize(); i++ ) {
            RECORD_STATS(maintenanceMessages++;
                         numBytesMaintenance += putMsg->getByteLength());

            sendRouteRpcCall(TIER1_COMP, lookupMsg->getSiblings(i),
                             new DHTPutCall(*putMsg));
        }

        delete putMsg;
    }
}

void DHT::finishApp()
{
    simtime_t time = globalStatistics->calcMeasuredLifetime(creationTime);

    if (time >= GlobalStatistics::MIN_MEASURED) {
        globalStatistics->addStdDev("DHT: Sent Maintenance Messages/s",
                                    maintenanceMessages / time);
        globalStatistics->addStdDev("DHT: Sent Normal Messages/s",
                                    normalMessages / time);
        globalStatistics->addStdDev("DHT: Sent Maintenance Bytes/s",
                                    numBytesMaintenance / time);
        globalStatistics->addStdDev("DHT: Sent Normal Bytes/s",
                                    numBytesNormal / time);
    }
}

int DHT::resultValuesBitLength(DHTGetResponse* msg) {
    int bitSize = 0;
    for (uint i = 0; i < msg->getResultArraySize(); i++) {
        bitSize += msg->getResult(i).getValue().size();

    }
    return bitSize;
}

