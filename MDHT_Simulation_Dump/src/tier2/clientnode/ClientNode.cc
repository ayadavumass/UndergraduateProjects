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
 * @file ClientNode.cc
 * @author Ingmar Baumgart
 */

#include <IPAddressResolver.h>
#include <GlobalNodeListAccess.h>
#include <GlobalStatisticsAccess.h>
#include <UnderlayConfiguratorAccess.h>
#include <RpcMacros.h>
#include "CommonMessages_m.h"

#include <GlobalDhtTestMap.h>

#include "ClientNode.h"

Define_Module(ClientNode);

using namespace std;

ClientNode::~ClientNode()
{
    cancelAndDelete(clientNode_put_timer);
    cancelAndDelete(clientNode_get_timer);
    cancelAndDelete(clientNode_mod_timer);
    cancelAndDelete(clientNode_get_sequence_timer);
    cancelAndDelete(clientNode_human_read_timer);
    cancelAndDelete(initial_put_timer);
}

ClientNode::ClientNode()
{
	clientNode_put_timer = NULL;
	clientNode_get_timer = NULL;
	clientNode_mod_timer = NULL;
    initial_put_timer = NULL;
    clientNode_get_sequence_timer = NULL;
    clientNode_human_read_timer = NULL;
}

void ClientNode::initializeApp(int stage)
{
    if (stage != MIN_STAGE_APP)
        return;

    // fetch parameters
    debugOutput = par("debugOutput");
    activeNetwInitPhase = par("activeNetwInitPhase");

    mean = par("testInterval");
    deviation = mean / 10;

    ttl = par("testTtl");

    globalNodeList = GlobalNodeListAccess().get();
    underlayConfigurator = UnderlayConfiguratorAccess().get();
    globalStatistics = GlobalStatisticsAccess().get();

    globalDhtTestMap = dynamic_cast<GlobalDhtTestMap*>(simulation.getModuleByPath(
            "globalObserver.globalFunctions[0].function"));

    if (globalDhtTestMap == NULL) {
        throw cRuntimeError("ClientNode::initializeApp(): "
                                "GlobalDhtTestMap module not found!");
    }

    // statistics
    numSent = 0;
    numGetSent = 0;
    numGetError = 0;
    numGetSuccess = 0;
    numPutSent = 0;
    numPutError = 0;
    numPutSuccess = 0;

    //initRpcs();
    WATCH(numSent);
    WATCH(numGetSent);
    WATCH(numGetError);
    WATCH(numGetSuccess);
    WATCH(numPutSent);
    WATCH(numPutError);
    WATCH(numPutSuccess);

    nodeIsLeavingSoon = false;
    firstGet = true;
    numberOfInlineObjects = 0;

    // initiate test message transmission
    clientNode_put_timer = new cMessage("clientNode_put_timer");
    clientNode_get_timer = new cMessage("clientNode_get_timer");
    clientNode_mod_timer = new cMessage("clientNode_mod_timer");
    initial_put_timer = new  cMessage("initial_put_timer");
    clientNode_get_sequence_timer = new cMessage("clientNode_get_sequence_timer");
    clientNode_human_read_timer = new cMessage("clientNode_human_read");

    if (mean > 0) {
        scheduleAt(simTime() + truncnormal(mean, deviation),
        		clientNode_put_timer);
        scheduleAt(simTime() + truncnormal(mean + mean / 3,
                                                      deviation),
                                                      clientNode_get_sequence_timer);
        /*scheduleAt(simTime() + truncnormal(mean + 2 * mean / 3,
                                                      deviation),
                                                      dhttestmod_timer);*/
    }
}

void ClientNode::handleRpcResponse(BaseResponseMessage* msg,
                                   cPolymorphic* context, int rpcId,
                                   simtime_t rtt)
{
    RPC_SWITCH_START(msg)
    RPC_ON_RESPONSE( DHTputCAPI ) {
        handlePutResponse(_DHTputCAPIResponse,
                          check_and_cast<DHTStatsContext*>(context));
        EV << "[ClientNode::handleRpcResponse()]\n"
           << "    DHT Put RPC Response received: id=" << rpcId
           << " msg=" << *_DHTputCAPIResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_ON_RESPONSE(DHTgetCAPI)
    {
        handleGetResponse(_DHTgetCAPIResponse,
                          check_and_cast<DHTStatsContext*>(context));
        EV << "[ClientNode::handleRpcResponse()]\n"
           << "    DHT Get RPC Response received: id=" << rpcId
           << " msg=" << *_DHTgetCAPIResponse << " rtt=" << rtt
           << endl;
        break;
    }
    RPC_SWITCH_END()
}

void ClientNode::handlePutResponse(DHTputCAPIResponse* msg,
                                   DHTStatsContext* context)
{
    if (!(msg->getIsSuccess())) {
        if (context->measurementPhase == false) {
            // don't count response, if the request was not sent
            // in the measurement phase
            RECORD_STATS(numPutError++);
        }
        delete context;
        return;
    }

    if (simTime().dbl() - context->requestTime < ttl) {
      OverlayKey key = msg->getKey();
      DHTEntry entry = {BinaryValue(msg->getValue()),
              context->requestTime + ttl};

      globalDhtTestMap->insertEntry(msg->getKey(), entry);
    } else {
      cout << "ohnein" << endl;
    }

    if (context->measurementPhase == false) {
        // don't count response, if the request was not sent
        // in the measurement phase
        delete context;
        return;
    }

    RECORD_STATS(numPutSuccess++);

    RECORD_STATS(globalStatistics->addStdDev("ClientNode: PUT delay (s)",
                                             SIMTIME_DBL(simTime() - context->requestTime)));
    delete context;
}

void ClientNode::handleGetResponse(DHTgetCAPIResponse* msg,
                                   DHTStatsContext* context)
{

	// is this the first getresponse of a sequence?
	if (firstGet) {
		firstGet = false;
		// Clientrequests on Userrequest
		numberOfInlineObjects = (int) lognormal(2.154982, 1.377062);
    		if (!clientNode_get_timer->isScheduled()) {
    			float timeBetweenClientRequests = (float) (lognormal(11.171529, 2.928586) + 78) / 100000;
    			EV << "CLIENTNODE !!!!!!!  created getTimer at simtime + " << timeBetweenClientRequests
    			   << endl;
    			scheduleAt(simTime()+timeBetweenClientRequests, clientNode_get_timer);
    		}
		numberOfInlineObjects--;

	}

    if (context->measurementPhase == false) {
    	EV << "Not in measurementPhase\n";
        // don't count response, if the request was not sent
        // in the measurement phase
        delete context;
        return;
    }

    RECORD_STATS(globalStatistics->addStdDev("ClientNode: GET delay (s)",
                                             SIMTIME_DBL(simTime() - context->requestTime)));

    if (!(msg->getIsSuccess())) {
        //cout << "ClientNode: success == false" << endl;
        RECORD_STATS(numGetError++);
        delete context;
        return;
    }

    const DHTEntry* entry = globalDhtTestMap->findEntry(context->key);

    if (entry == NULL) {
        //this key doesn't exist anymore in the DHT or never did
        delete context;

        if (msg->getResultArraySize() > 0) {
            RECORD_STATS(numGetError++);
            //cout << "ClientNode: deleted key still available" << endl;
            return;
        } else {
            RECORD_STATS(numGetSuccess++);
            //cout << "ClientNode: success (1)" << endl;
            return;
        }
    } else {
        delete context;
        if ((msg->getResultArraySize() > 0) && (msg->getResult(0).getValue() != entry->value) || !msg->getIsSuccess()) {
            RECORD_STATS(numGetError++);
            //cout << "ClientNode: wrong value" << endl;
            //cout << "size: " << msg->getResult(0).getValue().size() << endl;
            return;
        } else {
            RECORD_STATS(numGetSuccess++);
            //cout << "ClientNode: success (2)" << endl;
            return;
        }
    }

    EV << "ClientNode::handleGetResponse end\n";
}

void ClientNode::handleTraceMessage(cMessage* msg)
{
    char* cmd = new char[strlen(msg->getName()) + 1];
    strcpy(cmd, msg->getName());

    if (strlen(msg->getName()) < 5) {
        delete[] cmd;
        delete msg;
        return;
    }

    if (strncmp(cmd, "PUT ", 4) == 0) {
        // Generate key
        char* buf = cmd + 4;

        while (!isspace(buf[0])) {
            if (buf[0] == '\0')
                throw cRuntimeError("Error parsing PUT command");
            buf++;
        }

        buf[0] = '\0';
        BinaryValue b(cmd + 4);
        OverlayKey destKey(OverlayKey::sha1(b));

        // get value
        buf++;

        // build putMsg
        DHTputCAPICall* dhtPutMsg = new DHTputCAPICall();
        dhtPutMsg->setKey(destKey);
        dhtPutMsg->setValue(buf);
        dhtPutMsg->setTtl(ttl);
        dhtPutMsg->setIsModifiable(true);
        RECORD_STATS(numSent++; numPutSent++);
        sendInternalRpcCall(TIER1_COMP, dhtPutMsg,
                new DHTStatsContext(globalStatistics->isMeasuring(),
                                    simTime(), destKey));
    } else if (strncmp(cmd, "GET ", 4) == 0) {
        // Get key
        BinaryValue b(cmd + 4);
        OverlayKey key(OverlayKey::sha1(b));

        DHTgetCAPICall* dhtGetMsg = new DHTgetCAPICall();
        dhtGetMsg->setKey(key);
        RECORD_STATS(numSent++; numGetSent++);
        sendInternalRpcCall(TIER1_COMP, dhtGetMsg,
                new DHTStatsContext(globalStatistics->isMeasuring(),
                                    simTime(), key));
    } else {
        throw cRuntimeError("Unknown trace command; "
                                "only GET and PUT are allowed");
    }

    delete[] cmd;
    delete msg;
}

void ClientNode::handleTimerEvent(cMessage* msg)
{
    if (msg->isName("clientNode_put_timer")) {
        // schedule next timer event
        scheduleAt(simTime() + truncnormal(mean, deviation), msg);

        // do nothing if the network is still in the initialization phase
        if (((!activeNetwInitPhase) && (underlayConfigurator->isInInitPhase()))
                || underlayConfigurator->isSimulationEndingSoon()
                || nodeIsLeavingSoon)
            return;

        // create a put test message with random destination key
        OverlayKey destKey = OverlayKey::random();
        DHTputCAPICall* dhtPutMsg = new DHTputCAPICall();
        dhtPutMsg->setKey(destKey);
        dhtPutMsg->setValue(generateRandomValue());
        dhtPutMsg->setTtl(ttl);
        dhtPutMsg->setIsModifiable(true);

        RECORD_STATS(numSent++; numPutSent++);
        sendInternalRpcCall(TIER1_COMP, dhtPutMsg,
                new DHTStatsContext(globalStatistics->isMeasuring(),
                                    simTime(), destKey));

    } else if (msg->isName("clientNode_get_sequence_timer")) {
    	// a new sequence of get-requests starts
    	// number of userrequests per browsingsession
    	// TODO komische werte
    	numberOfGetRequests = (int) lognormal(1.792736, 1.147842) + 1;
    	EV << "CLIENTNODE Number of getRequests: " << numberOfGetRequests
    	   << endl;
    	firstGet = true;
        if (!clientNode_get_timer->isScheduled())
            scheduleAt(simTime(), clientNode_get_timer);
    } else if (msg->isName("clientNode_get_timer")) {
    	        // create new TimeoutTimer
        //dhttestget_timeout = new cMessage("dhttest_get_timeout");
        //scheduleAt(simTime() + getTimeout, dhttestget_timeout);

        if (numberOfInlineObjects > 0 and ! firstGet ) {
        	float timeBetweenClientRequests = (float) (lognormal(11.171529, 2.928586) + 78) / 100000;
        	scheduleAt(simTime() + timeBetweenClientRequests, msg);
        	numberOfInlineObjects--;
        	EV << "CLIENTNODE !!!!!!!  created getTimer at simtime + " << timeBetweenClientRequests
        	   << endl;
        }

        //TODO muss ansich in die getResponse gebaut werden
        if (numberOfInlineObjects == 0 and !firstGet) {
        	double humanReadTime = weibull(106.420313, 0.795353);
        	if (!clientNode_human_read_timer->isScheduled())
        		scheduleAt(simTime() + humanReadTime, clientNode_human_read_timer);
        	EV << "CLIENTNODE !!!!!!! created humanRead Timer: " << humanReadTime
        	   << endl;

        }


        // do nothing if the network is still in the initialization phase
        if ((!activeNetwInitPhase) && (underlayConfigurator->isInInitPhase())
                || underlayConfigurator->isSimulationEndingSoon()
                || nodeIsLeavingSoon) {
            return;
        }

        const OverlayKey& key = globalDhtTestMap->getRandomKey();
        //const OverlayKey& key = OverlayKey::random();

        if (key.isUnspecified())
            return;

        DHTgetCAPICall* dhtGetMsg = new DHTgetCAPICall();
        dhtGetMsg->setKey(key);
        RECORD_STATS(numSent++; numGetSent++);

        sendInternalRpcCall(TIER1_COMP, dhtGetMsg,
                new DHTStatsContext(globalStatistics->isMeasuring(),
                                    simTime(), key));
    } else if (msg->isName("clientNode_mod_timer")) {
        scheduleAt(simTime() + truncnormal(mean, deviation), msg);

        // do nothing if the network is still in the initialization phase
        if (((!activeNetwInitPhase) && (underlayConfigurator->isInInitPhase()))
                || underlayConfigurator->isSimulationEndingSoon()
                || nodeIsLeavingSoon) {
            return;
        }

        const OverlayKey& key = globalDhtTestMap->getRandomKey();

        if (key.isUnspecified())
            return;

        DHTputCAPICall* dhtPutMsg = new DHTputCAPICall();
        dhtPutMsg->setKey(key);
        dhtPutMsg->setValue(generateRandomValue());
        dhtPutMsg->setTtl(ttl);
        dhtPutMsg->setIsModifiable(true);

        RECORD_STATS(numSent++; numPutSent++);
        sendInternalRpcCall(TIER1_COMP, dhtPutMsg,
                new DHTStatsContext(globalStatistics->isMeasuring(),
                                    simTime(), key));
    } else if (msg->isName("clientNode_human_read")) {
    	numberOfGetRequests--;
    	firstGet = true;
    	if (numberOfGetRequests > 0)
    		if (!clientNode_get_timer->isScheduled())
                        scheduleAt(simTime(), clientNode_get_timer);
    	else {
    		double browsingInterSessionTime = weibull(54.902209, 0.7437);
    		// browsingInterSessionTime is given in Seconds, but we need minutes
    		if (!clientNode_get_sequence_timer->isScheduled())
    			scheduleAt(simTime() + (browsingInterSessionTime * 60) + (14 * 60), clientNode_get_sequence_timer);

    	}
}

}


BinaryValue ClientNode::generateRandomValue()
{
    char value[DHTTESTAPP_VALUE_LEN + 1];

    for (int i = 0; i < DHTTESTAPP_VALUE_LEN; i++) {
        value[i] = intuniform(0, 25) + 'a';
    }

    value[DHTTESTAPP_VALUE_LEN] = '\0';
    return BinaryValue(value);
}

void ClientNode::handleNodeLeaveNotification()
{
    nodeIsLeavingSoon = true;
}

void ClientNode::finishApp()
{
    simtime_t time = globalStatistics->calcMeasuredLifetime(creationTime);

    if (time >= GlobalStatistics::MIN_MEASURED) {
        // record scalar data
        globalStatistics->addStdDev("ClientNode: Sent Total Messages/s",
                                    numSent / time);
        globalStatistics->addStdDev("ClientNode: Sent GET Messages/s",
                                    numGetSent / time);
        globalStatistics->addStdDev("ClientNode: Failed GET Requests/s",
                                    numGetError / time);
        globalStatistics->addStdDev("ClientNode: Successful GET Requests/s",
                                    numGetSuccess / time);

        globalStatistics->addStdDev("ClientNode: Sent PUT Messages/s",
                                    numPutSent / time);
        globalStatistics->addStdDev("ClientNode: Failed PUT Requests/s",
                                    numPutError / time);
        globalStatistics->addStdDev("ClientNode: Successful PUT Requests/s",
                                    numPutSuccess / time);

        if (numGetSent > 0) {
            globalStatistics->addStdDev("ClientNode: GET Success Ratio",
                                        (double) numGetSuccess
                                        / (double) numGetSent);
        }
    }
}

