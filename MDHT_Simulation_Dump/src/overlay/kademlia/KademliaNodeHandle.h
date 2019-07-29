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

#ifndef __KADEMLIA_NODE_HANDLE_H
#define __KADEMLIA_NODE_HANDLE_H

#include <NodeHandle.h>
#include <NodeVector.h>

/**
 * @file KademliaNodeHandle.h
 * @author Sebastian Mies, Ingmar Baumgart, Bernhard Heep
 */
class KademliaBucketEntry : public ProxNodeHandle
{
public:
    /**
     * Constructs an unspecified NodeHandle
     */
    KademliaBucketEntry()
    : ProxNodeHandle()
    {
        staleCount = 0;
    }

    KademliaBucketEntry(const NodeHandle& handle, simtime_t prox = MAXTIME)
    : ProxNodeHandle(handle)
    {
        staleCount = 0;
        this->prox.proximity = SIMTIME_DBL(prox);
        this->prox.accuracy = 1.0;
    }

    /**
     * Complete constructor.
     *
     * @param key The OverlayKey
     * @param ip The IPvXAddress
     * @param port The UDP-Port
     */
    KademliaBucketEntry(const OverlayKey& key,
                        const IPvXAddress& ip,
                        int port)
    : ProxNodeHandle(NodeHandle(key, ip, port))
    {
        staleCount = 0;
    }

    // TODO
    inline simtime_t getRtt() const { return getProx(); } //deprecated
    inline void setRtt(simtime_t rtt) { this->prox.proximity = SIMTIME_DBL(rtt);  this->prox.accuracy = 1; } //deprecated

    inline uint8_t getStaleCount() const { return staleCount; }

    inline void setStaleCount(uint8_t staleCount) { this->staleCount = staleCount; }

    inline void resetStaleCount() { this->setStaleCount(0); }

    inline void incStaleCount() { this->staleCount++; }

    inline void setLastSeen(simtime_t lastSeen) { this->lastSeen = lastSeen; }

    inline simtime_t getLastSeen() { return this->lastSeen; }

private:

    uint8_t staleCount;
    simtime_t lastSeen;

    friend std::ostream& operator<<(std::ostream& os,
                                    const KademliaBucketEntry& n)
    {
        os << (NodeHandle)n << " " << n.prox.proximity;
        return os;
    };
};

/**
 * Class for extracting the relevant OverlayKey from a type used as template
 * parameter T_key for NodeVector<T, T_key> - Version for KademliaBucketEntry.
 *
 * @author Sebastian Mies
 * @author Felix M. Palmen
 */
template <> struct KeyExtractor<KademliaBucketEntry>
{
    static const OverlayKey& key(const KademliaBucketEntry& entry)
    {
        return entry.getKey();
    };
};

template <> struct ProxExtractor<KademliaBucketEntry>
{
    static Prox prox(const KademliaBucketEntry& entry)
    {
        return entry.getProx();
    };
};


#endif
