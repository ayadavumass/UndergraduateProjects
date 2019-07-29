/*
 * MultipleOverlayAccess.h
 *
 *  Created on: 07.01.2010
 *      Author: Michael Schwarz
 */

#ifndef MULTIPLEOVERLAYACCESS_H_
#define MULTIPLEOVERLAYACCESS_H_

#include <omnetpp.h>

#include <BaseOverlay.h>
#include <NotificationBoard.h>

/**
 * Gives access to the overlay.
 */
class MultipleOverlayAccess
{
public:

    BaseOverlay* get
        (cModule* refMod, int index)
    {
        // obtains the overlay related to the module, taking in account the index in case of SMOHs
        BaseOverlay *overlay = NULL;
        cModule *tmpMod = refMod;
        cModule *tmpParent = NULL; // parent of tmpMod

        if (index < 0) {
        	EV << "MultipleOverlayAccess::get() index is less than 0, setting to 0" << endl;
        	index = 0;
        }

        // go up from refMod until we get a NotificationBoard module, then we're at root
        // this will fail if the overlay is not in a container module!
        while (true) {
            tmpParent = tmpMod->getParentModule(); // get parent
            // search for a "notificationBoard" module
            cModule *notBoard = tmpParent->getSubmodule("notificationBoard");
            // is this a real NotificationBoard? then we're at root
            if (dynamic_cast<NotificationBoard*>(notBoard) != NULL) break;
            tmpMod = tmpParent; // else keep going up
            if (!tmpParent) throw cRuntimeError("MultipleOverlayAccess::get(): Overlay module not found!");
        }
        // get the overlay container, with the proper index
        
        int numLevels = refMod->par("numLevels");
        cModule *overlayContainer = tmpParent->getSubmodule("overlay", numLevels - 1 - index);
        overlay = dynamic_cast<BaseOverlay*>
                (overlayContainer->gate("appIn")->getNextGate()->getOwnerModule()); // get the contained overlay module

        if (!overlay) throw cRuntimeError("MultipleOverlayAccess::get(): Overlay module not found!");

        return overlay;
    }
};

#endif
