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
 * @file ScribeTest.cc
 * @author Stephan Krause
 */

#include "ScribeTest.h"

Define_Module(ScribeTest);

void ScribeTest::handleMessage( cMessage* msg )
{
    if( msg == timer ) {
        double random = uniform( 0, 1 );
        if( random < 0.1 || groupNum < 1 ) {
            joinGroup( ++groupNum );
        } else if( random < 0.2 ) {
            leaveGroup( groupNum-- );
        } else {
            sendDataToGroup( intuniform( 1, groupNum ));
        }
        scheduleAt( simTime() + 10, timer );

    } else if( ALMMulticastMessage* mcast = dynamic_cast<ALMMulticastMessage*>(msg) ){
        handleMCast( mcast );
    } else if( CompReadyMessage* readyMsg = dynamic_cast<CompReadyMessage*>(msg) ){
        if( readyMsg->getReady() ) {
            groupNum = 0;
            cancelEvent( timer );
            scheduleAt( simTime() + 1, timer );
        }
        delete msg;
    }
}

void ScribeTest::joinGroup(int i)
{
    ALMSubscribeMessage* msg = new ALMSubscribeMessage;
    msg->setGroupId(OverlayKey(i));
    send(msg, "to_lowerTier");
}

void ScribeTest::leaveGroup(int i)
{
    ALMLeaveMessage* msg = new ALMLeaveMessage;
    msg->setGroupId(OverlayKey(i));
    send(msg, "to_lowerTier");
}

void ScribeTest::sendDataToGroup( int i )
{
    ALMMulticastMessage* msg = new ALMMulticastMessage("FOO!");
    msg->setGroupId(OverlayKey(i));
    send(msg, "to_lowerTier");
}

void ScribeTest::handleMCast( ALMMulticastMessage* mcast )
{
    getParentModule()->getParentModule()->bubble("Received message!");
    EV << "[ScribeTest::handleMCast()]\n"
       << "    App received data message for group: " << mcast->getGroupId()
       << endl;
    delete mcast;
}
