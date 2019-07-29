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

#include "NetInfGlobalDataStorage.h"

#include <assert.h>
#include <cmath>

#include <algorithm>

#include <GlobalStatisticsAccess.h>

Define_Module(NetInfGlobalDataStorage);

void NetInfGlobalDataStorage::initialize()
{
  numLevels = par("numLevels");
  numLevels -= 1;
  targetOverlayTerminalNum = par("targetOverlayTerminalNum");
  objsPerNode = par("objsPerNode");
  levelProbability = par("levelProbability");
  zipfAlpha = par("zipfAlpha");
  popularityReversalTime = par("popularityReversalTime");
  lastReversal = 0;

  globalStatistics = GlobalStatisticsAccess().get();

  nStored = 0;
  initializedLinks = false;

  // Initialize all objects
  for (int i = 0; i < targetOverlayTerminalNum * objsPerNode; i++) {
    Entry entry;
    entry.key = OverlayKey::random();
    entry.state = UNUSED;
    entry.version = 0;
    entry.rank = i;

    entry.ownerString = "";

    dataStorage.insert(std::make_pair(entry.key, entry));
    ranks.push_back(entry.key);
  }
  assert(dataStorage.size() == ranks.size());
  assert(dataStorage.size() == (unsigned int) (targetOverlayTerminalNum * objsPerNode));
}

void NetInfGlobalDataStorage::finish()
{
  UsageMap::const_iterator uiter;
  for (uiter = usageMap.begin(); uiter != usageMap.end(); uiter++)
    globalStatistics->addStdDev("NetInfGlobalDataStorage: Number of entries per node", uiter->second);

  DataStorage::const_iterator diter;
  for (diter = dataStorage.begin(); diter != dataStorage.end(); diter++) {
    globalStatistics->addStdDev("NetInfGlobalDataStorage: Number of links per entry", diter->second.linkedEntries.size());
    if (diter->second.state == STORED)
      globalStatistics->addStdDev("NetInfGlobalDataStorage: Number of modifications per entry", diter->second.version);
  }
  globalStatistics->addStdDev("NetInfGlobalDataStorage: Nodes with stored entries", usageMap.size());
  globalStatistics->addStdDev("NetInfGlobalDataStorage: Number of entries", nStored);

  levelsStorage.clear();
  dataStorage.clear();
  ranks.clear();
  nStored = 0;
  initializedLinks = false;
  usageMap.clear();
}

NetInfGlobalDataStorage::Entry* NetInfGlobalDataStorage::getEntry(const OverlayKey& key)
{
  DataStorage::iterator iter = dataStorage.find(key);
  
  if (iter == dataStorage.end())
    return NULL;

  return &iter->second;
}

std::vector<std::string>* NetInfGlobalDataStorage::getPrefixes(const std::string& stringName)
{
  std::vector<std::string>* prefixes = new std::vector<std::string>();
  prefixes->push_back("");
  if (numLevels == 0)
    return prefixes;

  for (size_t i = 0; i < stringName.length(); i++)
    if (stringName[i] == '.')
      prefixes->push_back(stringName.substr(0, i));
  prefixes->push_back(stringName);

  return prefixes;
}

void NetInfGlobalDataStorage::reversePopularity()
{
  int n;

  DataStorage::iterator diter;
  n = ranks.size();
  ranks.clear();
  ranks.resize(n);
  for (diter = dataStorage.begin(); diter != dataStorage.end(); diter++) {
    diter->second.rank = n - diter->second.rank - 1;
    ranks[diter->second.rank] = diter->second.key;
  }

  LevelsStorage::iterator lsiter;
  for (lsiter = levelsStorage.begin(); lsiter != levelsStorage.end(); lsiter++) {
    LevelStorage::iterator liter;
    std::list<Entry*> entries;

    for (liter = lsiter->second.first.begin(); liter != lsiter->second.first.end(); liter++) {
      Entry * entry = getEntry(liter->second);
      entries.push_back(entry);
    }
    lsiter->second.first.clear();
    lsiter->second.second = false;

    std::list<Entry*>::iterator eiter;
    for (eiter = entries.begin(); eiter != entries.end(); eiter++)
      lsiter->second.first.insert(std::make_pair((*eiter)->rank, (*eiter)->key));
  }
  lastReversal = SIMTIME_DBL(simTime());
}

NetInfGlobalDataStorage::Entry* NetInfGlobalDataStorage::getRandomEntryForGet(const NodeHandle& handle, const std::string& stringName)
{
  if (isSeeding())
    return NULL;

  if (simTime() - lastReversal > popularityReversalTime) {
    reversePopularity();
  }

  // 1) get prefix
  std::vector<std::string>* prefixes = getPrefixes(stringName);
  int p;
  p = (levelProbability == 0.0) ? prefixes->size() : (levelProbability >= 1.0 ? 0 : geometric(levelProbability));
  if (prefixes->size() >= (unsigned int) (p + 1))
    p = prefixes->size() - p - 1;
  else
    p = 0;

  // 2) get lowest prefix with keys
  LevelsStorage::iterator lsiter;
  do {
    assert(p >= 0);
    lsiter = levelsStorage.find((*prefixes)[p]);
    p--;
  } while (lsiter == levelsStorage.end());
  p++;

  delete prefixes;
  
  // 3) initialize level if necessary
  if (!lsiter->second.second) {
    lsiter->second.second = true;    

    // Calculate integral to build zipf probability distribution
    unsigned int i;
    double I = 0.0;
    for (i = 1; i <= lsiter->second.first.size(); i++) {
      I += (1.0 / (pow(i, zipfAlpha)));
    }
    I = 1.0 / I;

    // Calculate probability of every entry in this level
    double s = 0.0;
    LevelStorage::iterator liter;
    for (i = 1, liter = lsiter->second.first.begin(); liter != lsiter->second.first.end(); liter++, i++) {
      Entry *e2 = getEntry(liter->second);
      assert(e2 != NULL);

      s += I / pow(i, zipfAlpha);
      if (e2->probability.size() <= (unsigned int) p)
        e2->probability.resize(p+1, 0.0);
      e2->probability[p] = s;
    }
  }

  // 4) get key
  double u = uniform(0, 1);
  for (LevelStorage::iterator liter = lsiter->second.first.begin(); liter != lsiter->second.first.end(); liter++) {
    Entry *e2 = getEntry(liter->second);
    assert(e2 != NULL);

    if (e2->probability[p] >= u) {
      return e2;
    }
  }
  assert(false);
}

NetInfGlobalDataStorage::Entry* NetInfGlobalDataStorage::getRandomEntryForPut(const NodeHandle& handle, const std::string& stringName, int userIdx)
{
  if (nStored == dataStorage.size())
    return NULL;

  UsageMap::iterator uiter = usageMap.find(stringName);
  if (uiter == usageMap.end())
    uiter = usageMap.insert(std::make_pair(stringName, 0)).first;
  if (uiter->second >= 1.1 * objsPerNode)
    return NULL;
  uiter->second += 1;

  int idx = intuniform(0, ranks.size()-1);
  const OverlayKey& key = ranks[idx];
  DataStorage::iterator iter = dataStorage.find(key);
  while (iter != dataStorage.end() && iter->second.state != UNUSED)
    iter++;
  if (iter == dataStorage.end()) {
    iter = dataStorage.begin();

    while (iter != dataStorage.end() && iter->second.state != UNUSED)
      iter++;
    // Must not happen because otherwise nStored == dataStorage.size()
    assert (iter != dataStorage.end());
  }

  assert(iter->second.state == UNUSED);
  iter->second.state = PENDING;
  {
    std::ostringstream os;
    os << stringName << ":" << userIdx;
    iter->second.ownerString = os.str();
  }
  nStored++;

  std::vector<std::string>* prefixes = getPrefixes(stringName);

  for (std::vector<std::string>::const_iterator piter = prefixes->begin(); piter != prefixes->end(); piter++) {
    LevelsStorage::iterator lsiter = levelsStorage.find(*piter);
      
    if (lsiter == levelsStorage.end()) {
      lsiter = levelsStorage.insert(std::make_pair(*piter, std::make_pair(LevelStorage(), false))).first;
    }
    lsiter->second.second = false;
    lsiter->second.first.insert(std::make_pair(iter->second.rank, iter->second.key));
  }
  delete prefixes;


  if (!isSeeding() && !initializedLinks) {
    initializedLinks = true;

    DataStorage::iterator diter;
    for (diter = dataStorage.begin(); diter != dataStorage.end(); diter++)
      initializeLinkedEntries(diter->second);
  }

  return &iter->second;
}

void NetInfGlobalDataStorage::initializeLinkedEntries(Entry& entry)
{
  // A WEB BROWSING WORKLOAD MODEL FOR SIMULATION
  //   Lourens O. Walters
  // Table 6.14
  // Number of Web Client requests per Web User Requests
  
  int m = std::max(std::min((int) lognormal(2.154982, 1.377062), 200), 0);
  std::string stringName = entry.ownerString.substr(0, entry.ownerString.find(':'));
  std::vector<std::string>* prefixes = getPrefixes(stringName);
  
  for (int i = 0; i < m; i++) {
    int p;
    p = (levelProbability == 0.0) ? prefixes->size() : (levelProbability >= 1.0 ? 0 : geometric(levelProbability));
    if (prefixes->size() >= (unsigned int) (p + 1))
      p = prefixes->size() - p - 1;
    else
      p = 0;
    
    LevelsStorage::iterator lsiter;
    do {
      assert(p >= 0);
      lsiter = levelsStorage.find((*prefixes)[p]);
      p--;
    } while (lsiter == levelsStorage.end());
    p++;
    
    int lastRank = (lsiter->second.first.rbegin())->first;
    int r = intuniform(0, lastRank);
    
    entry.linkedEntries.push_back(lsiter->second.first.lower_bound(r)->second);
  }
  delete prefixes;
}

void NetInfGlobalDataStorage::notifyStored(const NodeHandle& handle, const std::string& ownerString, const OverlayKey& key)
{
  Entry * entry = getEntry(key);
  
  assert(entry != NULL);
  assert(entry->state != UNUSED);
  assert(entry->ownerString == ownerString);

  if (entry->state == PENDING) {
    entry->state = STORED;
  }
  
  if (!isSeeding()) {
    if (entry->linkedEntries.size() == 0)
      initializeLinkedEntries(*entry);
  }
}

