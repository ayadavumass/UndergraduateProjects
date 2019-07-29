//
// Copyright (C) 2009 Institut fuer Telematik, Universitaet Karlsruhe (TH)
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
 * @file NiceCluster.cc
 * @author Christian Huebsch
 */


#include "NiceCluster.h"

// Constructor *****************************************************************
NiceCluster::NiceCluster()
{
    WATCH_SET(cluster);
    WATCH(leader);

    setLeader(TransportAddress::UNSPECIFIED_NODE);

    last_LT = 0.0;

} // NICECluster

// Adds member to cluster ******************************************************
void NiceCluster::add( const TransportAddress& member )
{

    cluster.insert(member);

} // add
//
// Clears all cluster contents *************************************************
void NiceCluster::clear()
{

    cluster.clear();
    setLeader(TransportAddress::UNSPECIFIED_NODE);

    last_LT = 0.0;

} // clear

// Check if cluster member *****************************************************
bool NiceCluster::contains( const TransportAddress& member )
{

    ConstClusterIterator it = cluster.find(member);

    if (it != cluster.end())
        return true;

    return false;

} // contains
//
// Get cluster size ************************************************************
int NiceCluster::getSize()
{

    return cluster.size();

} // getSize

// Get address of specific member **********************************************
const TransportAddress& NiceCluster::get( int i )
{

    ConstClusterIterator it = cluster.begin();

    for (int j = 0; j < i; j++)
        it++;

    return *it;

} // get

void NiceCluster::remove(const TransportAddress& member)
{

    cluster.erase(member);

} // remove

// set cluster leader **********************************************************
void NiceCluster::setLeader(const TransportAddress& leader)
{

    this->leader = leader;

//	//cout << "Leader set to " << leader << endl;

} // setLeader

// get current cluster leader **************************************************
const TransportAddress& NiceCluster::getLeader()
{

    return leader;

} // getLeader


simtime_t NiceCluster::get_Last_LT()
{

    return last_LT;

}

void NiceCluster::set_Last_LT()
{

    last_LT = simTime();

}

