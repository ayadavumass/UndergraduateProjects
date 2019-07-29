//
// Copyright (C) 2008 Institut fuer Telematik, Universitaet Karlsruhe (TH)
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
 * @file Landmark.cc
 * @author Fabian Hartmann, Bernhard Heep
 */

#include <cassert>

#include <SimpleUnderlayConfigurator.h>
#include <BootstrapList.h>
//#include <CoordBasedRouting.h>
#include <NeighborCache.h>
#include <GlobalNodeList.h>
#include <GlobalStatistics.h>
//#include <CoordMessages_m.h>

#include "Landmark.h"

Define_Module(Landmark);

Landmark::~Landmark() {
}

void Landmark::initializeApp(int stage)
{
    if (stage != MIN_STAGE_APP)
        return;

    SimpleNodeEntry* entry =
        dynamic_cast<SimpleInfo*>(globalNodeList->
                                  getPeerInfo(thisNode.getAddress()))->getEntry();

    //globalNodeList->setOverlayReadyIcon(getThisNode(), false);

    // Get the responsible Landmark churn generator
    ChurnGenerator* lmChurnGen = NULL;
    for (uint8_t i = 0; i < underlayConfigurator->getChurnGeneratorNum(); i++) {
        ChurnGenerator* searchedGen;
        searchedGen = underlayConfigurator->getChurnGenerator(i);
        if (searchedGen->getNodeType().overlayType == "oversim.common.cbr.LandmarkModules") {
            lmChurnGen = searchedGen;
        }
    }

    // If number of landmarks < dim+1: Place magic landmark
    // Else: BaseOverlay will take care of the coordinate calculation
    //if (!neighborCache->coordBasedRouting) return;

    //uint8_t dim = neighborCache->/*coordBasedRouting->*/getXmlDimensions();
    //if (lmChurnGen && lmChurnGen->terminalCount < dim + 1) {
        //PeerInfo* thisInfo = globalNodeList->getPeerInfo(getThisNode());

    if (true) { //TODO
        // magic placement using underlays coords
        //std::vector<double> coords;
        for (uint8_t i = 0; i < entry->getDim(); i++) {
            ownCoords.push_back(entry->getCoords(i));
        }

        //neighborCache->nps->setOwnCoordinates(coords);
        neighborCache->getNpsAccess().setOwnLayer(0);

        thisNode = overlay->getThisNode();
        globalNodeList->setOverlayReadyIcon(getThisNode(), true);
        globalNodeList->refreshEntry(getThisNode());
    } else {
        //TODO
    }

    WATCH_VECTOR(ownCoords);
}

/*
void Landmark::joinOverlay() {
    // Coordinate calculation via BaseOverlay finished
    // Simple Layer 1 node now, ignoring NodeID stuff
    globalNodeList->setOverlayReadyIcon(getThisNode(), true);
    globalNodeList->refreshEntry(getThisNode());
    state = READY;
}
*/

void Landmark::finishApp()
{
    if (neighborCache->getNpsAccess().getReceivedCalls() != 0) {
        globalStatistics->recordOutVector("Calls to Landmarks",
            neighborCache->getNpsAccess().getReceivedCalls());
    }
}

/*
void Landmark::handleUDPMessage(BaseOverlayMessage* msg) {
    std::cout << "I received something!" << endl;
}

void Landmark::handleTimerEvent(cMessage* msg) {
}

void Landmark::handleRpcResponse(BaseResponseMessage *msg,
                                     cPolymorphic* context,
                                     int rpcId, simtime_t rtt) {
}

void Landmark::handleRpcTimeout(BaseCallMessage *msg,
                                    const TransportAddress &dest,
                                    cPolymorphic* context, int rpcId,
                                    const OverlayKey &destKey) {
}

void Landmark::updateTooltip() {
}

NodeVector* Landmark::findNode(const OverlayKey& key,
                             int numRedundantNodes,
                             int numSiblings,
                             BaseOverlayMessage* msg) {
    NodeVector* dummy = new NodeVector(0);
    return dummy;
}
*/
/*
bool Landmark::handleRpcCall(BaseCallMessage* msg)
{
    RPC_SWITCH_START( msg );
        RPC_DELEGATE( CoordsOfNode, coordsOfNodeRpc);
#ifdef EXTJOIN_DISCOVERY
        RPC_DELEGATE( RttToNode, rttToNodeRpc);
#endif
    RPC_SWITCH_END( );

    return RPC_HANDLED;// || BaseRpc::internalHandleRpcCall(msg);
}

void Landmark::coordsOfNodeRpc(CoordsOfNodeCall* msg)
{
    neighborCache->incReceivedCalls();
    CoordsOfNodeResponse* coordRes = new CoordsOfNodeResponse("CoordsOfNodeRes");
    coordRes->setLayer(neighborCache->getOwnLayer());
    std::vector<double> ownCoordinates = neighborCache->getOwnCoordinates();
    uint dim = neighborCache->getXmlDimensions();
    coordRes->setCoordsArraySize(dim);
    for (uint i = 0; i < ownCoordinates.size(); i++) {
        coordRes->setCoords(i, ownCoordinates[i]);
    }
//    coordRes->setLength(COORDRESPONSE_L(coordRes));
    sendRpcResponse(msg, coordRes);
}
*/
