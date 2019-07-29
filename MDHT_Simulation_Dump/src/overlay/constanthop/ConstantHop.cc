//
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

#include <iostream>
#include <string>
#include <map>

#include <GetLevel.h>

#include "ConstantHop_m.h"

#include "ConstantHop.h"

Define_Module(ConstantHop);

ConstantHop::ConstantHop() {}

ConstantHop::~ConstantHop() {}

typedef std::map<OverlayKey, NodeHandle> NodeMap;
typedef std::map<std::string, NodeMap> SegmentMap;
static SegmentMap peers;

void ConstantHop::initializeOverlay(int stage)
{
    if (stage != MIN_STAGE_OVERLAY) return;

    hops = par("hops");
    thisLevel = GetLevel().get(this);
    numLevels = par("numLevels");

    hostnameInfo = check_and_cast<HostnameInfo*>
                   (simulation.getModuleByPath("globalObserver.globalFunctions[1].function"));
    assert(hostnameInfo != NULL);

    if (numLevels <= 1 || thisLevel == numLevels-1) {
      thisSegment = "";
    } else {
      std::string s = hostnameInfo->getHostname(thisNode.getAddress());

      int i, j = 0;
      for (i = 0; i < s.length(); i++) {
        if (s[i] == '.') {
          j++;
          if (j == (numLevels - thisLevel - 1)) {
            s = s.substr(0, i);
            break;
          }
        }
      }
      thisSegment = s;
    }

    OverlayKey thisKey = hostnameInfo->getKey(thisNode.getAddress(), thisSegment);
    thisNode.setKey(thisKey);
}

void ConstantHop::joinOverlay()
{
    SegmentMap::iterator siter = peers.find(thisSegment);
    if (siter == peers.end())
      siter = peers.insert(std::make_pair(thisSegment, NodeMap())).first;

    siter->second.insert(std::make_pair(thisNode.getKey(), thisNode));

    setOverlayReady(true);
}

bool ConstantHop::isSiblingFor(const NodeHandle& node,
                               const OverlayKey& key,
                               int numSiblings,
                               bool* err)
{
    SegmentMap::iterator siter = peers.find(thisSegment);
    assert(siter != peers.end());

    NodeMap::iterator niter = siter->second.lower_bound(key);
    // key <= niter
    if (niter == siter->second.end())
      niter = siter->second.begin();
    *err = false;

    return niter->second == node;
}

NodeVector *ConstantHop::findNode(const OverlayKey& key,
                                  int numRedundantNodes,
                                  int numSiblings,
                                  BaseOverlayMessage* msg)
{
    NodeVector *nextHops = new NodeVector(1);

    ConstantHopFindNodeExtMessage *fne;
    int remainingHops;
      
    if (!msg->hasObject("findNodeExt")) {
      fne = new ConstantHopFindNodeExtMessage("findNodeExt");
      fne->setRemainingHops(hops);
      msg->addObject(fne);
    }
    fne = (ConstantHopFindNodeExtMessage*) msg->getObject("findNodeExt");
    remainingHops = fne->getRemainingHops();

    SegmentMap::iterator siter = peers.find(thisSegment);
    assert(siter != peers.end());

    OverlayKey nh = (remainingHops <= 1) ? key : OverlayKey::random();
    
    NodeMap::iterator niter = siter->second.lower_bound(nh);
    // key <= niter
    if (niter == siter->second.end())
      niter = siter->second.begin();
    nextHops->push_back(niter->second);
    fne->setRemainingHops(remainingHops - 1);

    return nextHops;
}

void ConstantHop::finishOverlay()
{
    setOverlayReady(false);

    SegmentMap::iterator siter = peers.find(thisSegment);
    siter->second.erase(thisNode.getKey());
}

int ConstantHop::getMaxNumSiblings()
{
    return 1;
}

int ConstantHop::getMaxNumRedundantNodes()
{
    return 1;
}

