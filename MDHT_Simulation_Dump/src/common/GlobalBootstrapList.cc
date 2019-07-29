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
 * @file BootstrapList.cc
 * @author Bin Zheng, Ingmar Baumgart
 */

#include <BaseApp.h>
#include <BootstrapList.h>
#include <GlobalBootstrapList.h>
#include <GlobalNodeList.h>
#include <BaseOverlay.h>
#include <UnderlayConfiguratorAccess.h>
#include <ZeroconfConnector.h>
#include <CommonMessages_m.h>
#include <hashWatch.h>

#include <fstream>
#include <iostream>

using namespace std;

Define_Module(GlobalBootstrapList);

GlobalBootstrapList::GlobalBootstrapList()
{

}


GlobalBootstrapList::~GlobalBootstrapList()
{

}


void GlobalBootstrapList::initialize()
{
    numLevels = par("numLevels");
}

void GlobalBootstrapList::handleMessage(cMessage *msg)
{
	cancelAndDelete(msg);
	return;
}

const NodeHandle* GlobalBootstrapList::getBootstrapNode(int level, const std::string& segment)
{
	if (level == 0 and numLevels > 1) {
		EV << "[GlobalBootstrapList::getBootstrapNode()] level: " << level << " segment: " << segment << " , returning empty nodehandle" << endl;
		const NodeHandle *bootstrapNode = &NodeHandle::UNSPECIFIED_NODE;
		return bootstrapNode;
	} else {
		const NodeHandle *bootstrapNode = &NodeHandle::UNSPECIFIED_NODE;
		int i, j = 0;

                std::map<std::pair<int, std::string>, BootstrapNodeSet>::iterator iter2;
                iter2 = bootstrapList.find(make_pair(level, segment));

		if (iter2 != bootstrapList.end() && !iter2->second.empty()) {
			i = intrand(iter2->second.size());
			BootstrapNodeSet::iterator iter = iter2->second.begin();

			while (j < i) {
				iter++;
				j++;
			}

			bootstrapNode = (NodeHandle *)iter->second;
		}
		EV << "[GlobalBootstrapList::getBootstrapNode()] level: " << level << " segment: " << segment << " , returning: " << *bootstrapNode << endl;
		return bootstrapNode;
	}
}

bool GlobalBootstrapList::insertBootstrapCandidate(BootstrapNodeHandle& node,
											 int level, const std::string& segment)
{

    EV <<  "[GlobalBootstrapList::insertBootstrapCandidate()] " << node << " with level: " << level  << " segment: " << segment << endl;

    Enter_Method_Silent();

    BootstrapNodeSet::iterator iter;

    node.setLastPing(simTime());

    std::map<std::pair<int, std::string>, BootstrapNodeSet>::iterator iter2;

    iter2 = bootstrapList.find(make_pair(level, segment));

    // if node already in the list, update its lastPing and nodePrio fields
    if (iter2 != bootstrapList.end() && (iter = iter2->second.find(node)) != iter2->second.end()) {
        iter->second->setLastPing(node.getLastPing());
        iter->second->setNodePrio(node.getNodePrio());

        return true;
    }

    bootstrapList.insert (make_pair(make_pair(level, segment), BootstrapNodeSet()));
    iter2 = bootstrapList.find(make_pair(level, segment));
    iter2->second.insert(NodePair(node, &node));

    return false;
}



void GlobalBootstrapList::finish()
{
	//TODO cleanup
}
