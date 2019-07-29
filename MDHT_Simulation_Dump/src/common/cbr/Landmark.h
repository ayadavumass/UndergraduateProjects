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
 * @file Landmark.h
 * @author Fabian Hartmann
 */

#ifndef __LANDMARK_H_
#define __LANDMARK_H_

#include <BaseApp.h>

class CoordsOfNodeCall;

class Landmark : public BaseApp
{
  private:
    virtual ~Landmark();

    virtual void initializeApp(int stage);
    virtual void finishApp();

    std::vector<double> ownCoords;

  public:
    const std::vector<double>& getOwnNpsCoords() const { return ownCoords; };

      //virtual void handleUDPMessage(BaseOverlayMessage* msg);
    //virtual void handleTimerEvent(cMessage* msg);
    //bool handleRpcCall(BaseCallMessage* msg);
    //void coordsOfNodeRpc(CoordsOfNodeCall* msg);
    //virtual void handleRpcResponse(BaseResponseMessage *msg,
    //                               cPolymorphic* context,
    //                               int rpcId, simtime_t rtt);
    //virtual void handleRpcTimeout(BaseCallMessage *msg,
    //                              const TransportAddress &dest,
    //                              cPolymorphic* context, int rpcId,
    //                              const OverlayKey &destKey);

    //virtual void joinOverlay();
    //virtual NodeVector* findNode(const OverlayKey& key,
    //                             int numRedundantNodes,
    //                             int numSiblings,
    //                             BaseOverlayMessage* msg);

    //virtual bool isSiblingFor(const NodeHandle& node,
    //                          const OverlayKey& key, int numSiblings, bool* err) { return false; };

    /**
     * updates information shown in tk-environment
     */
    //virtual void updateTooltip();

};

#endif
