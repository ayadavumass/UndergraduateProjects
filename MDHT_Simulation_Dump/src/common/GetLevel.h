/*
 * GetLevel.h
 *
 *  Created on: 11.01.2010
 *      Author: Michael Schwarz
 */

#ifndef GETLEVEL_H_
#define GETLEVEL_H_

#include <omnetpp.h>

/**
 * Get's the level of the given Module.
 */
class GetLevel
{
public:

    int get
        (cModule* refMod)
    {
    	// Evil Hack
    	// TODO muss mal schoener gemacht werden
    	int level = -1;
        int numLevels = refMod->par("numLevels");

    	std::string tmpStr = refMod->getFullPath();
    	//EV << "GetLevel::get() tmpStr=" << tmpStr << endl;

    	// Prüfen ob mehrere Module existieren, ansonsten gibts nur ein level
    	if (tmpStr.rfind("[") == tmpStr.find("["))
    		return numLevels - 1;

    	tmpStr = tmpStr.substr(tmpStr.rfind("[") + 1, tmpStr.rfind("]") - tmpStr.rfind("[") - 1);

    	level = atoi(tmpStr.c_str());
    	return numLevels - 1 - level;

    }
};


#endif /* GETLEVEL_H_ */
