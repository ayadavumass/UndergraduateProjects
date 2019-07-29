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

cplusplus {{
#include <OverlayKey.h>
#include <IPvXAddress.h>
#include <CommonMessages_m.h>
}}

class CommonAPIMessage;
class BaseCallMessage;
class BaseResponseMessage;

class noncobject OverlayKey;
class noncobject IPvXAddress;

//
// @author Bernhard Heep
//
packet KBRTestMessage
{
    int id;                 // the message ID
    int seqNum;             // the sequence number of the message
    bool measurementPhase;  // if false, this msg doesn't count for statistics
    IPvXAddress visitedNodes[];
}

//
// A basic find node rpc response
//
packet KbrTestCall extends BaseCallMessage
{
    //OverlayKey destKey;     // request nextHops for this key
    //bool measurementPhase;  // if false, this msg doesn't count for statistics
}

//
// A basic find node rpc call
//
packet KbrTestResponse extends BaseResponseMessage
{
    //bool measurementPhase;  // if false, this msg doesn't count for statistics
}

class KbrRpcContext
{
    OverlayKey destKey;     // request nextHops for this key
    bool measurementPhase;  // if false, this msg doesn't count for statistics
}
