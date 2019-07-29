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
 * @file NiceCluster.h
 * @author Christian Huebsch
 */

#ifndef __NICECLUSTER_H_
#define __NICECLUSTER_H_

#include <BaseOverlay.h>
#include <omnetpp.h>

class NiceCluster
{

private:

    /* set of cluster members */
    std::set<TransportAddress> cluster;
    /* leader of the cluster */
    TransportAddress leader;
    simtime_t last_LT;

    typedef std::set<TransportAddress>::const_iterator ConstClusterIterator;
    typedef std::set<TransportAddress>::iterator ClusterIterator;

public:

    NiceCluster();
    void add(const TransportAddress& member); // adds member to cluster
    void clear(); // clears all cluster contents
    bool contains( const TransportAddress& member ); //check if member
    int getSize(); // get cluster size
    const TransportAddress& get( int i ); // get address of specific member
    void remove(const TransportAddress& member);

    // set and get leader for this cluster
    void setLeader(const TransportAddress& leader);
    const TransportAddress& getLeader();

    simtime_t get_Last_LT();
    void set_Last_LT();

}; // NiceCluster

#endif /* _NICECLUSTER_H_ */
