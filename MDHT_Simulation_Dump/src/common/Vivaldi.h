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
 * @file Vivaldi.h
 * @author Jesus Davila Marchena
 * @author Bernhard Heep
 */

#ifndef _VIVALDI_
#define _VIVALDI_


#include <vector>

#include <omnetpp.h>

#include <GlobalStatisticsAccess.h>
#include <NeighborCache.h>
#include <CoordinateSystem.h>

class TransportAddress;


class Vivaldi
{
  private:
    //variables for storing the parameter from ned file
    double errorC;
    double coordC;
    bool enableHeightVector;
    double lossC;
    double effectiveSample;
    uint32_t dimension;

    // variables for storing the node coordinates / estimated error
    std::vector<double> xi;
    double errori;
    double loss;
    double lossResetLimit;

  protected:
    bool useSVivaldi;
    bool showVivaldiPosition;

    virtual void finishVivaldi();

    virtual void updateDisplay();

    //pointer to GlobalStatistics
    GlobalStatistics* globalStatistics;
    NeighborCache* neighborCache;

  public:
    virtual void initVivaldi(NeighborCache* neighborCache);
    void processCoordinates(simtime_t rtt,
                            const std::vector<double>& xj,
                            double errrorj);
    Prox getCoordinateBasedProx(const NodeCoordsInfo& info);
    const std::vector<double>& getOwnCoordinates() const { return xi; };
    inline double getOwnError() const { return errori; };
};

#endif
