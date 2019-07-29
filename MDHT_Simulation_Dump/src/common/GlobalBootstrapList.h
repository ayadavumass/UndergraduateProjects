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
 * @file BootstrapList.h
 * @author Bin Zheng, Ingmar Baumgart
 */


#ifndef __GLOBALBOOTSTRAPLIST_H_
#define __GLOBALBOOTSTRAPLIST_H_

#include <omnetpp.h>
#include <BootstrapNodeHandle.h>
#include <NodeVector.h>
#include <oversim_mapset.h>

#include <map>

class BaseOverlay;
class ZeroconfConnector;

typedef std::pair<TransportAddress, BootstrapNodeHandle*> NodePair;
// hash_map for accommodating bootstrap nodes
typedef UNORDERED_MAP<TransportAddress, BootstrapNodeHandle*,
                      TransportAddress::hashFcn> BootstrapNodeSet;

class GlobalBootstrapList : public cSimpleModule
{

public:
	GlobalBootstrapList();
    ~GlobalBootstrapList();

    const NodeHandle* getBootstrapNode(int level, const std::string& segment);

   bool insertBootstrapCandidate(BootstrapNodeHandle& node, int level = 0, const std::string& segment = "");

private:
    std::map<std::pair<int, std::string>, BootstrapNodeSet> bootstrapList;

    virtual void initialize();

    virtual void finish();

    virtual void handleMessage(cMessage* msg);

    int numLevels;
};

#endif
