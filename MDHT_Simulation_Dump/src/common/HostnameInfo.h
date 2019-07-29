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

#ifndef __HOSTNAME_INFO_H__
#define __HOSTNAME_INFO_H__

#include <omnetpp.h>
#include <string>
#include <map>

#include <IPvXAddress.h>
#include <OverlayKey.h>

class HostnameInfo : public cSimpleModule
{
public:
  std::string getHostname(const IPvXAddress& handle);
  OverlayKey getKey(const IPvXAddress& handle, const std::string& segment);
  void addNode(const IPvXAddress& parent, const IPvXAddress& address, bool leaf);

protected:
  void initialize();
  void finish();

private:
  // properties
  int underlayMode;
  int numLevels;
  bool sameKeys;
  int *levelConfiguration;

  std::map<IPvXAddress, std::pair<std::string, bool> > nodeNameMap;
  std::map<std::string, int> namesCounter;

  std::map<std::pair<IPvXAddress, std::string>, OverlayKey> keys;
};

#endif /* __HOSTNAME_INFO_H__ */
