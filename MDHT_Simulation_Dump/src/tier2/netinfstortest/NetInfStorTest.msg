//
// Copyright (C) 2010 Sebastian Dröge <slomo@upb.de>
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
// @author Sebastian Dröge
//

cplusplus {{
#include <CommonMessages_m.h>
}}

class noncobject OverlayKey;

message NetInfStorTestDelayMessage
{
  OverlayKey key;

  int userIdx;
}

// To schedule user get actions
message NetInfStorTestUserMessage
{
  int userIdx;
  bool start;
}

// To remove user locators
message NetInfStorTestRemoveLocatorMessage
{
  OverlayKey key;
  int userIdx;
}

// To update entry versions of entries we own
message NetInfStorTestUpdateEntryMessage
{
  OverlayKey key;
  int userIdx;
}
