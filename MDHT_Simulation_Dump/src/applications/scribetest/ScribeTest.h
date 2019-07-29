//
// Copyright (C) 2006 Institut fuer Telematik, Universitaet Karlsruhe (TH)
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
 * @file ScribeTest.h
 * @author Stephan Krause
 */


#ifndef __SCRIBETEST_H_
#define __SCRIBETEST_H_

#include <omnetpp.h>
#include "CommonMessages_m.h"

class ScribeTest : public cSimpleModule {
    public:
        ScribeTest() { timer = new cMessage( "app_timer"); }
        ~ScribeTest() { cancelAndDelete( timer ); }
    protected:
        void handleMessage( cMessage* msg );
        void joinGroup(int i);
        void leaveGroup(int i);
        void sendDataToGroup(int i);
        void handleMCast( ALMMulticastMessage* mcast );
        cMessage* timer;
        int groupNum;
};

#endif
