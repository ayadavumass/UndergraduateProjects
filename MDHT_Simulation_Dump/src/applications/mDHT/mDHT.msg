cplusplus {{
#include <OverlayKey.h>
#include <BinaryValue.h>
#include <CommonMessages_m.h>
}}

class noncobject OverlayKey;
class noncobject BinaryValue;
class noncobject NodeHandle;
class noncobject TransportAddress;
class noncobject SimTime;
class BaseCallMessage;
class BaseResponseMessage;

//
// Tier internal messages
//

packet mDHTPutCall extends BaseCallMessage
{
  OverlayKey key;

  // primary locator
  string ownerString;

  // increment when the BO changes
  int version;

  int level;
}

packet mDHTPutResponse extends BaseResponseMessage
{
  OverlayKey key;
  bool success;

  int level;

  //
  // For statistics
  //

  // Hierarchical path taken
  TransportAddress path[];
}

packet mDHTAddLocatorCall extends BaseCallMessage
{
  OverlayKey key;

  // primary locator
  string ownerString;

  // secondary locator to add
  string locatorString;

  // version for the secondary locator
  int version;

  int level;
}

packet mDHTAddLocatorResponse extends BaseResponseMessage
{
  OverlayKey key;
  bool success;

  int level;

  //
  // For statistics
  //

  // Hierarchical path taken
  TransportAddress path[];
}

packet mDHTRemoveLocatorCall extends BaseCallMessage
{
  OverlayKey key;

  string locatorString;

  int level;
}

packet mDHTRemoveLocatorResponse extends BaseResponseMessage
{
  OverlayKey key;
  bool success;

  int level;

  //
  // For statistics
  //

  // Hierarchical path taken
  TransportAddress path[];
}

packet mDHTGetCall extends BaseCallMessage
{
  OverlayKey key;

  string originator;
  TransportAddress originatorHandle;
  int originatorRpcId;

  int level;
}

packet mDHTGetResponse extends BaseResponseMessage
{
  OverlayKey key;
  int ttl;
  bool success;

  // primary locator
  string ownerString;

  // chosen secondary locator
  string locatorString;

  int version;

  int level;

  //
  // For statistics
  //

  // Hierarchical path taken
  TransportAddress path[];
}

packet mDHTFastDataCall extends BaseCallMessage
{
  OverlayKey key;
  int ttl;
  bool success;

  // primary locator
  string ownerString;

  // chosen secondary locator
  string locatorString;

  int version;

  int originatorRpcId;

  int level;
}

packet mDHTFastDataResponse extends BaseResponseMessage
{

}

//
// Internal self messages
//

message mDHTTtlMessage
{
  OverlayKey key;
}

message mDHTDelayMessage
{
  // 0 - put
  // 1 - get
  // 2 - add locator
  // 3 - remove locator
  // 4 - fast data
  int delayedType;

  bool request; // true == request, false == response

  int rpcId;

  OverlayKey key;
  bool success;

  TransportAddress path[];

  // for get only
  string ownerString;
  string locatorString;
  int version;
  int ttl;
}

//
// Application API
//

packet NetInfAPIPutCall extends BaseCallMessage
{
  OverlayKey key;

  int ownerUser;

  // increment whenever the BO changes
  int version;
}

packet NetInfAPIPutResponse extends BaseResponseMessage
{
  OverlayKey key;
  bool success;

  string ownerString;
}

packet NetInfAPIAddLocatorCall extends BaseCallMessage
{
  OverlayKey key;

  // Primary locator
  // fails if a different copy is found later
  string ownerString;

  // secondary locator
  int locatorUser;
  // version of the secondary locator
  int version;
}

packet NetInfAPIAddLocatorResponse extends BaseResponseMessage
{
  OverlayKey key;
  bool success;

  string locatorString;
}

packet NetInfAPIRemoveLocatorCall extends BaseCallMessage
{
  OverlayKey key;

  int locatorUser;
}

packet NetInfAPIRemoveLocatorResponse extends BaseResponseMessage
{
  OverlayKey key;
  bool success;

  string locatorString;
}

packet NetInfAPIGetCall extends BaseCallMessage
{
  OverlayKey key;

  int originatorUser;
}

packet NetInfAPIGetResponse extends BaseResponseMessage
{
  OverlayKey key;
  int ttl;
  bool success;

  // primary locator
  string ownerString;

  // chosen secondary locator
  string locatorString;

  int version;
}

