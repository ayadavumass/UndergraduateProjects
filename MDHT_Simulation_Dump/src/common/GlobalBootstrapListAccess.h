#ifndef __GLOBALBOOTSTRAPLISTACCESS_H__
#define __GLOBALBOOTSTRAPLISTACCESS_H__


#include <omnetpp.h>
#include "GlobalBootstrapList.h"


/**
 * Gives access to the GlobalNodeList.
 */
class GlobalBootstrapListAccess
{
public:
	GlobalBootstrapList* get()
    {
        return (GlobalBootstrapList*) simulation.getModuleByPath("globalObserver.globalBootstrapList");
    }
};

#endif

