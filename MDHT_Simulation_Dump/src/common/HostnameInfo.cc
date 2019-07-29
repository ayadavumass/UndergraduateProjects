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

#include "HostnameInfo.h"
#include <sstream>

#include <assert.h>

Define_Module(HostnameInfo);

void HostnameInfo::initialize()
{
  sameKeys = par("sameKeys");
  underlayMode = par("underlayMode");

  if (underlayMode == 0) {
    numLevels = par("numLevels");
    numLevels -= 1;
    if (numLevels > 0) {
      const std::string& levelConfigurationStr = par("levelConfiguration");

      levelConfiguration = new int[numLevels-1];

      int currentNumber = 0, j = 0;
      for (unsigned int i = 0; i < levelConfigurationStr.length() && j < numLevels-1; i++) {
        if (levelConfigurationStr[i] == ' ') {
          levelConfiguration[j++] = currentNumber;
          currentNumber = 0;
         continue;
        }

        assert(levelConfigurationStr[i] >= '0' && levelConfigurationStr[i] <= '9');
        currentNumber *= 10;
        currentNumber += levelConfigurationStr[i] - '0';
      }
      if (currentNumber != 0) {
        levelConfiguration[j++] = currentNumber;
      }

      assert (j == numLevels-1);
    } else {
      levelConfiguration = 0;
    }
  } else if (underlayMode == 1) {

  }
}

void HostnameInfo::finish()
{
  nodeNameMap.clear();
  namesCounter.clear();
  keys.clear();
}

std::string HostnameInfo::getHostname(const IPvXAddress& handle)
{
  {
    std::map<IPvXAddress, std::pair<std::string, bool> >::const_iterator it;

    it = nodeNameMap.find(handle);
    if (it != nodeNameMap.end()) {
      assert(it->second.second);
      return it->second.first;
    }
  }

  assert (underlayMode == 0);

  int i;
  std::ostringstream os;

  if (numLevels > 0) {
    for (i = 0; i < numLevels-1; i++) {
      os << intuniform(0, levelConfiguration[i] - 1) << ".";
    }

    std::map<std::string, int>::iterator iter = namesCounter.find(os.str());
    if (iter == namesCounter.end()) {
      namesCounter.insert(std::make_pair(os.str(), 0));
      os << "0";
    } else {
      iter->second++;
      os << iter->second;
    }
    nodeNameMap.insert(std::make_pair(handle, std::make_pair(os.str(), true)));

    return os.str();
  } else {
    std::ostringstream os;
    std::map<std::string, int>::iterator iter = namesCounter.find("");
    if (iter == namesCounter.end()) {
      namesCounter.insert(std::make_pair(os.str(), 0));
      os << "0";
    } else {
      iter->second++;
      os << iter->second;
    }
    nodeNameMap.insert(std::make_pair(handle, std::make_pair(os.str(), true)));

    return os.str();
  }
}

void HostnameInfo::addNode(const IPvXAddress& parent, const IPvXAddress& address, bool leaf)
{
  std::ostringstream os;
  std::string pstr;

  if (underlayMode == 0 || nodeNameMap.find(address) != nodeNameMap.end())
    return;

  if (parent.isUnspecified()) {
    pstr = "";
  } else {
    std::map<IPvXAddress, std::pair<std::string, bool> >::const_iterator it;

    it = nodeNameMap.find(parent);
    assert(it != nodeNameMap.end());
    pstr = it->second.first;
    os << pstr << ".";
  }

  std::map<std::string, int>::iterator iter = namesCounter.find(pstr);
  if (iter == namesCounter.end()) {
    namesCounter.insert(std::make_pair(pstr, 0));
    os << "0";
  } else {
    iter->second++;
    os << iter->second;
  }

  nodeNameMap.insert(std::make_pair(address, std::make_pair(os.str(), leaf)));
}

OverlayKey HostnameInfo::getKey(const IPvXAddress& handle, const std::string& segment)
{
  std::map<std::pair<IPvXAddress, std::string>, OverlayKey>::iterator iter;
  std::string s = segment;

  if (sameKeys)
    s = "";

  iter = keys.find(std::make_pair(handle, s));
  if (iter == keys.end()) {
    OverlayKey key = OverlayKey::random();
    keys.insert(std::make_pair(std::make_pair(handle, s), key));
    return key;
  } else {
    return iter->second;
  }
}

