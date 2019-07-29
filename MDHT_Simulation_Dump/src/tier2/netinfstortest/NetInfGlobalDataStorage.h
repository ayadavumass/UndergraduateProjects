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

#ifndef __NETINF_GLOBAL_DATA_STORAGE_H__
#define __NETINF_GLOBAL_DATA_STORAGE_H__

#include <omnetpp.h>

#include <OverlayKey.h>
#include <BinaryValue.h>
#include <NodeHandle.h>
#include <GlobalStatistics.h>

#include <HashFunc.h>
#include <string>
#include <set>
#include <vector>

class NetInfGlobalDataStorage : public cSimpleModule
{
public:
  enum EntryState {
    UNUSED,
    PENDING,
    STORED
  };

  struct Entry {
    OverlayKey key;

    EntryState state;
    int version;
    size_t rank;
    std::vector<OverlayKey> linkedEntries;

    std::string ownerString;

    // cached per-level probabilities
    std::vector<double> probability;
  };

  Entry* getEntry(const OverlayKey& key);
  Entry* getRandomEntryForGet(const NodeHandle& handle, const std::string& stringName);
  Entry* getRandomEntryForPut(const NodeHandle& handle, const std::string& stringName, int userIdx);

  void notifyStored(const NodeHandle& handle, const std::string& ownerString, const OverlayKey& key);

  bool isSeeding() { return (nStored < dataStorage.size()); }

protected:
  void initialize();
  void finish();

private:

  typedef UNORDERED_MAP<OverlayKey, Entry> DataStorage;
  DataStorage dataStorage;
  typedef std::map<int, OverlayKey> LevelStorage;
  typedef UNORDERED_MAP<std::string, std::pair<LevelStorage, bool> > LevelsStorage;
  LevelsStorage levelsStorage;
  std::vector<OverlayKey> ranks;  // global ranks
  unsigned int nStored;
  bool initializedLinks;

  typedef std::map<std::string, int> UsageMap;
  UsageMap usageMap;

  // properties
  int numLevels;
  int targetOverlayTerminalNum;
  int objsPerNode;
  double percentPreseed;
  double levelProbability;
  double zipfAlpha;
  int popularityReversalTime;

  int lastReversal;

  GlobalStatistics* globalStatistics;

  std::vector<std::string>* getPrefixes(const std::string& stringName);
  void initializeLinkedEntries(Entry& entry);

  void reversePopularity();
};

#endif /* __NETINF_GLOBAL_DATA_STORAGE_H__ */
