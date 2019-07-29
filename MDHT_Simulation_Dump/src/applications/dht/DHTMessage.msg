//
// Copyright (C) 2007 Institut fuer Telematik, Universitaet Karlsruhe (TH)
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

//
// @author Ingmar Baumgart
//

cplusplus {{
#include <OverlayKey.h>
#include <IPvXAddress.h>
#include <BinaryValue.h>
#include <NodeHandle.h>
#include <CommonMessages_m.h>

#define RESULT_L(msg) (resultValuesBitLength(msg) + msg->getResultArraySize() * (KEY_L + 2 * sizeof(uint32_t) + sizeof(int) + 2 * sizeof(bool) + NODEHANDLE_L))
#define PUTCALL_L(msg) (BASECALL_L(msg) + KEY_L + msg->getValue().size() * sizeof(char) + sizeof(int) + sizeof(int32_t) + sizeof(int32_t) + sizeof(bool) * 2 + NODEHANDLE_L)
#define GETCALL_L(msg) (BASECALL_L(msg) + sizeof(bool))
#define PUTRESPONSE_L(msg) (BASERESPONSE_L(msg) + KEY_L + msg->getValue().size() * sizeof(char))
#define GETRESPONSE_L(msg) (BASERESPONSE_L(msg) + KEY_L + msg->getHashValue().size() * sizeof(char) \
                            + RESULT_L(msg) + sizeof(bool))
}}

class BaseRpcMessage;
class BaseCallMessage;
class BaseResponseMessage;

class noncobject DhtDumpEntry;
class noncobject OverlayKey;
class noncobject IPvXAddress;
class noncobject BinaryValue;
class noncobject NodeHandle;

packet DHTPutCall extends BaseCallMessage
{
    OverlayKey key;
    BinaryValue value;
    uint32_t kind;
    uint32_t id;
    int ttl;
    bool isModifiable;
    bool maintenance;
    NodeHandle ownerNode;
}

packet DHTGetCall extends BaseCallMessage
{
    OverlayKey key;
    uint32_t kind;
    uint32_t id;
    bool isHash;
}

packet DHTPutResponse extends BaseResponseMessage
{
    OverlayKey key;
    BinaryValue value;
}

packet DHTGetResponse extends BaseResponseMessage
{
    OverlayKey key;
    BinaryValue hashValue;    
    bool isHash;
    DhtDumpEntry result[];
}

message DHTTtlTimer
{
    OverlayKey key;
    uint32_t kind;
    uint32_t id;
}
