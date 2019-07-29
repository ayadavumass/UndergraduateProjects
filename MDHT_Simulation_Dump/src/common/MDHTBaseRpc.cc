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

// Rewrite of certain BaseRpc methods to provide multi level support

#include "MDHTBaseRpc.h"

#include "MultipleOverlayAccess.h"
#include <OverlayAccess.h>
#include <GlobalStatisticsAccess.h>
#include <GetLevel.h>

MDHTBaseRpc::MDHTBaseRpc() {
    defaultRpcListener = NULL;
    neighborCache = NULL;
    cryptoModule = NULL;
}

//private
void MDHTBaseRpc::initRpcs()
{
    BaseRpc::initRpcs();

    // set friend modules
    globalStatistics = GlobalStatisticsAccess().get();

    rpcUdpTimeout = par("rpcUdpTimeout");
    rpcKeyTimeout = par("rpcKeyTimeout");
    rpcExponentialBackoff = par("rpcExponentialBackoff");

    numLevels = par("numLevels");

    rpcsPending = 0;
    rpcStates.clear();

    defaultRpcListener = new RpcListener();

    //set ping cache
    numPingSent = 0;
    bytesPingSent = 0;
    numPingResponseSent = 0;
    bytesPingResponseSent = 0;

    WATCH(numPingSent);
    WATCH(bytesPingSent);
    WATCH(numPingResponseSent);
    WATCH(bytesPingResponseSent);

    // Get the Level of this module
    int thisLevel = GetLevel().get(this);

    // set overlay pointer
    /*
    EV << "BaseRpc::initRpc: Id=" << this->getId() << "\n"
	   << "                  FullPath=" << this->getFullPath() << "\n"
	   << "                  thisLevel=" << thisLevel << endl;
	*/
    thisCompType = getThisCompType();

    if (thisCompType == TIER1_COMP || thisCompType == TIER2_COMP || thisCompType == TIER3_COMP) {
    	// We have to register this Component in every overlay!
    	for (int i = 0; i < numLevels; i++) {
    		overlay = MultipleOverlayAccess().get(this, i);
    		overlay->registerComp(thisCompType, this);
    	}
		overlay = MultipleOverlayAccess().get(this, 0);
    } else {
        overlay = MultipleOverlayAccess().get(this, 0);
		overlay->registerComp(thisCompType, this);
    }



    // get pointer to the neighborCache
    cModule *mod = getParentModule();
    while (neighborCache == NULL) {
        neighborCache = (NeighborCache*)mod->getSubmodule("neighborCache");
        mod = mod->getParentModule();
        if (!mod)
            throw cRuntimeError("BaseRpc::initRpc: "
                                    "Module type contains no ping cache!");
    }

    // get pointer to the cryptoModule
    mod = getParentModule();
    cryptoModule = NULL;
    while (cryptoModule == NULL) {
        cryptoModule = (CryptoModule*)mod->getSubmodule("cryptoModule");
        mod = mod->getParentModule();
        if (!mod)
            throw cRuntimeError("BaseRpc::initRpc: CryptoModule not found!");
    }
}

uint32_t MDHTBaseRpc::sendRpcCallToLevel(TransportType transportType,
                              CompType destComp,
                              int level,
                              const TransportAddress& dest,
                              const OverlayKey& destKey,
                              BaseCallMessage* msg,
                              cPolymorphic* context,
                              RoutingType routingType,
                              simtime_t timeout,
                              int retries,
                              int rpcId,
                              RpcListener* rpcListener)
{
    // create nonce, timeout and set default parameters
    uint32_t nonce;
    do {
        nonce = intuniform(1, 2147483647);
    } while (rpcStates.count(nonce) > 0);

    if (timeout == -1) {
        switch (transportType) {
        case INTERNAL_TRANSPORT:
            timeout = 0;
            break;
        case UDP_TRANSPORT:
            timeout = rpcUdpTimeout;
            break;
        case ROUTE_TRANSPORT:
            timeout = (destKey.isUnspecified() ?
                       rpcUdpTimeout :
                       rpcKeyTimeout);
            break;
        default:
            throw cRuntimeError("BaseRpc::sendRpcMessage(): "
                                    "Unknown RpcTransportType!");
        }
    }

    if (rpcListener == NULL)
        rpcListener = defaultRpcListener;

    // create state
    RpcState state;
    state.id = rpcId;
    state.timeSent = simTime();
    state.dest = dest.dup();
    state.destKey = destKey;
    state.srcComp = getThisCompType();
    state.destComp = destComp;
    state.listener = rpcListener;
    state.timeoutMsg = new RpcTimeoutMessage();
    state.timeoutMsg->setNonce(nonce);
    state.retries = retries;
    state.rto = timeout;
    state.transportType = transportType;
    //state.transportType = (destKey.isUnspecified() && (dest.getSourceRouteSize() == 0)
    //        ? UDP_TRANSPORT : transportType); //test
    state.routingType = routingType;
    state.context = context;

    if (rpcStates.count(nonce) > 0)
        throw cRuntimeError("RPC nonce collision");

    // set message parameters
    msg->setNonce(nonce);
    if (transportType == ROUTE_TRANSPORT)
        msg->setSrcNode(overlay->getThisNode());
    else
        msg->setSrcNode(thisNode);
    msg->setType(RPC);

    // sign the message
    // if (transportType != INTERNAL_TRANSPORT) cryptoModule->signMessage(msg);

    // save copy of call message in RpcState
    state.callMsg = dynamic_cast<BaseCallMessage*>(msg->dup());
    assert(!msg->getEncapsulatedMsg() || !msg->getEncapsulatedMsg()->getControlInfo());

    // register state
    rpcStates[nonce] = state;

    // schedule timeout message
    if (state.rto != 0)
        scheduleAt(simTime() + state.rto, state.timeoutMsg);

    // TODO: cleanup code to have only one type for source routes
    std::vector<TransportAddress> sourceRoute;
    sourceRoute.push_back(dest);
    if (dest.getSourceRouteSize() > 0) {
        state.transportType = transportType = ROUTE_TRANSPORT;
        sourceRoute.insert(sourceRoute.begin(), dest.getSourceRoute().rend(),
                          dest.getSourceRoute().rbegin());
        // remove the original source route from the destination
        sourceRoute.back().clearSourceRoute();
    }
    sendRpcMessageWithTransportToLevel(transportType, destComp, level, routingType,
                                sourceRoute, destKey, msg);

    return nonce;
}

void MDHTBaseRpc::sendRpcMessageWithTransportToLevel(TransportType transportType,
                                          CompType destComp,
                                          int level,
                                          RoutingType routingType,
                                          const std::vector<TransportAddress>& sourceRoute,
                                          const OverlayKey& destKey,
                                          BaseRpcMessage* message)
{
    switch (transportType) {
    case UDP_TRANSPORT: {
        sendMessageToUDP(sourceRoute[0], message);
        break;
    }
    case ROUTE_TRANSPORT: {
    	overlay=MultipleOverlayAccess().get(this, level);
        internalSendRouteRpc(message, destKey,
                             sourceRoute, routingType);
        overlay = MultipleOverlayAccess().get(this,0);
        break;
    }
    case INTERNAL_TRANSPORT: {
    	overlay=MultipleOverlayAccess().get(this, level);

        cGate *destCompGate = overlay->getCompRpcGate(destComp);
        EV << "sendRpcMessageWithTransport: destcompgate=" << destCompGate->getFullPath() << endl;
        if (destCompGate == NULL) {
            throw cRuntimeError("BaseRpc::sendRpcMessageWithTransport():"
                                    " INTERNAL_RPC to unknown RpcCompType!");
        }
        OverlayCtrlInfo *overlayCtrlInfo = new OverlayCtrlInfo();
        overlayCtrlInfo->setSrcComp(getThisCompType());
        overlayCtrlInfo->setDestComp(destComp);
        overlayCtrlInfo->setTransportType(INTERNAL_TRANSPORT);
        message->setControlInfo(overlayCtrlInfo);
        sendDirect(message, destCompGate);

        //reset overlay pointer
        overlay = MultipleOverlayAccess().get(this,0);
        break;
    }
    default:
        throw cRuntimeError("BaseRpc::sendRpcMessageWithTransport: "
                                "invalid transportType!");
        break;
    }
}
