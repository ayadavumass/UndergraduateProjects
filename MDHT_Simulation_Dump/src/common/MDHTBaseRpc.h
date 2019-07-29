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

#ifndef MDHTBASERPC_H_
#define MDHTBASERPC_H_

#include <BaseRpc.h>

class MDHTBaseRpc : public BaseRpc {
public:
	MDHTBaseRpc();
protected:
    /**
     * Sends a Remote-Procedure-Call message to the underlay.<br>
     * USE ONE OF THE WRAPPER FUNCTIONS INSTEAD: <br>
     * sendRouteRpcCall(), sendInternalRpcCall(), or sendUdpRpcCall()
     *
     * If no timeout is provided, a default value of
     * globalParameters.rpcUdpTimeout for
     * underlay and globalParameters.rpcKeyTimeout for a overlay rpc
     * is used. Internal RPCs don't have a default timeout.
     * After a timeout the message gets retransmitted for
     * at maximum retries times. The destKey
     * attribute is kept untouched.
     *
     * @param transportType The type of transport
     * @param destComp The destination component
     * @param level The level the packet should be sent to
     * @param dest Destination node handle (may contain a source route)
     * @param destKey route the RPC to the node that is
     *        responsible for destkey
     * @param msg RPC Call Message
     * @param context a pointer to an arbitrary cPolymorphic object,
     *        which can be used to store additional state
     * @param routingType KBR routing type
     * @param timeout RPC timeout in seconds (-1=use default value, 0=no timeout)
     * @param retries How often we try to resent rpc call, if it gets lost
     * @param rpcId RPC id
     * @param rpcListener RPC Listener (callback handler)
     * @return The nonce of the RPC
     */
    uint32_t sendRpcCallToLevel(TransportType transportType,
                         CompType destComp,
                         int level,
                         const TransportAddress& dest,
                         const OverlayKey& destKey,
                         BaseCallMessage* msg,
                         cPolymorphic* context,
                         RoutingType routingType,
                         simtime_t timeout, int retries,
                         int rpcId, RpcListener* rpcListener);


    void sendRpcMessageWithTransportToLevel(TransportType transportType,
                                     CompType destComp,
                                     int level,
                                     RoutingType routingType,
                                     const std::vector<TransportAddress>& sourceRoute,
                                     const OverlayKey& destKey,
                                     BaseRpcMessage* message);
protected:
    /**
     * Initializes Remote-Procedure state.
     */
    virtual void initRpcs();

	/**
	 * Sends an internal Remote-Procedure-Call between two tiers<br>
	 *
	 * If no timeout is provided, a default value of
	 * globalParameters.rpcUdpTimeout for
	 * underlay and globalParameters.rpcKeyTimeout for a overlay rpc
	 * is used. After a timeout the message gets retransmitted for
	 * at maximum retries times. The destKey
	 * attribute is kept untouched.
	 *
	 * @param destComp Destination component
	 * @param msg RPC Call Message
	 * @param context a pointer to an arbitrary cPolymorphic object,
	 *        which can be used to store additional state
	 * @param timeout RPC timeout in seconds (-1=use default value, 0=no timeout)
	 * @param retries How often we try to resent rpc call, if it gets lost
	 * @param rpcId RPC id
	 * @param rpcListener RPC Listener
	 * @return The nonce of the RPC
	 */
	inline uint32_t sendInternalRpcCallToLevel(CompType destComp,
			int level,
			BaseCallMessage* msg,
			cPolymorphic* context = NULL,
			simtime_t timeout = -1,
			int retries = 0,
			int rpcId = -1,
			RpcListener* rpcListener = NULL)
	{
		return sendRpcCallToLevel(INTERNAL_TRANSPORT, destComp, level,
				TransportAddress::UNSPECIFIED_NODE,
				OverlayKey::UNSPECIFIED_KEY, msg, context,
				NO_OVERLAY_ROUTING, timeout, retries, rpcId,
				rpcListener);
	}

	int numLevels;

};

#endif /* MDHTBASERPC_H_ */
