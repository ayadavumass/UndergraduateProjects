/*
 * MultipleDHTAccess.h
 *
 *  Created on: 29.12.2009
 *      Author: Michael Schwarz
 */

#ifndef MULTIPLEDHTACCESS_H_
#define MULTIPLEDHTACCESS_H_

#include <omnetpp.h>

#include <BaseApp.h>
#include <NotificationBoard.h>

/**
 * Gives access to the connected DHTs.
 */
class MultipleDHTAccess
{
public:

    BaseApp* get
        (cModule* refMod, int index)
    {
        // obtains the overlay related to the module, taking in account the index in case of SMOHs
    	BaseApp *dht = NULL;
        cModule *tmpMod = refMod;
        cModule *tmpParent = NULL; // parent of tmpMod

        // go up from refMod until we get a NotificationBoard module, then we're at root
        // this will fail if the overlay is not in a container module!
        while (true) {
            tmpParent = tmpMod->getParentModule(); // get parent
            // search for a "notificationBoard" module
            cModule *notBoard = tmpParent->getSubmodule("notificationBoard");
            // is this a real NotificationBoard? then we're at root
            if (dynamic_cast<NotificationBoard*>(notBoard) != NULL) break;
            tmpMod = tmpParent; // else keep going up
            if (!tmpParent) throw cRuntimeError("MultipleDHTAccess::get(): NotificationBoard module not found!");
        }
        // get the overlay container, with the proper index
        int numLevels = refMod->par("numLevels");
        cModule *overlayContainer = tmpParent->getSubmodule("tier1", numLevels - 1 - index);
        dht = dynamic_cast<BaseApp*>
                (overlayContainer->gate("from_upperTier")->getNextGate()->getOwnerModule()); // get the contained overlay module

        if (!dht) throw cRuntimeError("MultipleDHTAccess::get(): requested Tier1 module not found!");

        return dht;
    }
};

#endif /* MULTIPLEDHTACCESS_H_ */
