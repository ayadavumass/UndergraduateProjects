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
 * @file Vivaldi.cc
 * @author Jesus Davila Marchena
 * @author Bernhard Heep
 */


#include <cfloat>

#include <NeighborCache.h>

#include "Vivaldi.h"


void Vivaldi::initVivaldi(NeighborCache* neighborCache)
{
    this->neighborCache = neighborCache;

    useSVivaldi = neighborCache->par("useSVivaldi");
    errorC = neighborCache->par("vivaldiErrorConst");
    coordC = neighborCache->par("vivaldiCoordConst");
    dimension = neighborCache->par("vivaldiDimConst");
    enableHeightVector = neighborCache->par("vivaldiEnableHeightVector");
    lossC = neighborCache->par("vivaldiLossConst");
    effectiveSample = neighborCache->par("vivaldiEffectiveSample");
    showVivaldiPosition = neighborCache->par("showVivaldiPosition");
    lossResetLimit =  neighborCache->par("lossResetLimit");

    if(enableHeightVector) dimension += 1;

    // init variables
    xi.resize(dimension);
    errori = 1.0;
    loss = 0;

    for (uint32_t i = 0; i < dimension; i++) {
        xi[i] = uniform(-.2, .2); //TODO
    }
    if(enableHeightVector) xi[dimension - 1] = 0;

    WATCH(errori);
    WATCH_VECTOR(xi);
    WATCH(loss);

    globalStatistics = GlobalStatisticsAccess().get();
};

void Vivaldi::processCoordinates(simtime_t rtt,
                                 const std::vector<double>& xj,
                                 double errorj)
{
    //assert(rtt > 0.0);
    if (rtt <= 0.0) return;

    // init variables
    double dist = 0, sum = 0, delta = 0;
    double weight = (((errori + errorj) == 0) ?
                     0 : (errori / (errori + errorj)));

    uint32_t size = dimension;
    if(enableHeightVector) size -= 1;

    // calculate distance
    if (rtt != 0) {
        for (uint32_t i = 0; i < size; i++) {
            dist += pow(xi[i] - xj[i], 2);
        }
        dist = sqrt(dist);
    }
    if (enableHeightVector) dist += xi[size] + xj[size];

    // choose Vivaldi version
    if (useSVivaldi) {
        // get avg absolute prediction error
        sum = neighborCache->getAvgAbsPredictionError(xi, effectiveSample);

        // update weighted moving average of local error
        errori = (sum * errorC) + errori * (1 - errorC);

        // calculate loss factor
        loss = lossC + (1 - lossC) * loss;
        if(fabs(dist-SIMTIME_DBL(rtt)) > lossResetLimit) loss = 0.0;
        delta = coordC * weight * (1 - loss);
    } else {
        double relErr = 0;
        if (rtt != 0) {
            //eSample computes the relative error for this sample
            relErr = fabs(dist - rtt) / rtt;
        }
        // update weighted moving average of local error
        errori = (relErr * errorC * weight) + errori * (1 - errorC * weight);
        // estimates the delta factor
        delta = coordC * weight;
    }

    // update local coordinates
    if (dist > 0) {
        for (uint32_t i = 0; i < size; i++) {
            xi[i] += (delta * (SIMTIME_DBL(rtt) - dist)) * ((xi[i] - xj[i]) / dist);
        }
        if(enableHeightVector) {
            xi[size] += (delta * (SIMTIME_DBL(rtt) - dist));
            if(xi[size] < 0) xi[size] = 0.0;
        }
    }
    updateDisplay();
}

Prox Vivaldi::getCoordinateBasedProx(const NodeCoordsInfo& info)
{
    double dist = 0.0, accuracy = 0.0;
    uint32_t size = info.coordinates.size();
    if(enableHeightVector) size -= 1;

    for (uint32_t i = 0; i < size; i++) {
        dist += pow(xi[i] - info.coordinates[i], 2);
    }
    dist = sqrt(dist);

    accuracy = 1 - ((info.coordErr + errori) / 2);
    if (info.coordErr >= 1.0 || errori >= 1.0) accuracy = 0.0;
    if (accuracy < 0) accuracy = 0.0;
    if (accuracy > 1) accuracy = 1;

    if (enableHeightVector) return Prox(dist +
                                        xi[size] +
                                        info.coordinates[size],
                                        info.coordErr);
    return Prox(dist, accuracy);
}


void Vivaldi::updateDisplay()
{
    char buf[60];
    sprintf(buf, "xi[0]: %f xi[1]: %f ", xi[0], xi[1]);
    neighborCache->getDisplayString().setTagArg("t", 0, buf);

    // show nodes at estimated position TODO
    if (showVivaldiPosition) {
        for (uint32_t i = 0; i < dimension; i++)
            neighborCache->getParentModule()
                ->getDisplayString().setTagArg("p", i, xi[i] * 1000);
    }
}

void Vivaldi::finishVivaldi()
{
    globalStatistics->addStdDev("Vivaldi: Errori(ei)",errori);
}
