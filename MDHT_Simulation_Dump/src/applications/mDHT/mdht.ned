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

package oversim.applications.mDHT;

import oversim.common.BaseRpc;
import oversim.common.ITier;


//
// Simple module of the mDHT application
//
// @author Michael Schwarz
//
simple mDHT extends BaseRpc
{
    parameters:
        @class(mDHT);
        bool debugOutput;
	int numLevels;
	int cacheTtl;
	int copyRefreshTime;
	int maxCacheSize;
        bool passiveCache;
	string perLevelDelayFactor;

	int statisticsSampleInterval;
	bool enableTimeStatistics;
    gates:
        input udpIn;    // gate from the UDP layer
        input from_lowerTier[numLevels];    // gate from the lower tier
        input from_upperTier;    // gate from the upper tier
        input direct_in;    // gate for sendDirect
        input trace_in;    // gate for trace file commands
        output udpOut;    // gate to the UDP layer
        output to_lowerTier[numLevels];    // gate to the lower tier
        output to_upperTier;    // gate to the upper tier        
}


//
// Compound module for the mDHT application
//
// @author Michael Schwarz
//
module mDHTModules
{
    parameters:
        @display("i=block/segm");
        int numLevels;

    gates:
        input from_lowerTier[numLevels];    // gate from the lower tier
        input from_upperTier;    // gate from the upper tier
        input udpIn;    // gate from the UDP layer
        input trace_in;    // gate for trace file commands
        output to_lowerTier[numLevels];    // gate to the lower tier
        output to_upperTier;    // gate to the upper tier
        output udpOut;    // gate to the UDP layer

    submodules:
        mDHTModule: mDHT;
    connections allowunconnected:
        for i=0..numLevels-1 {
        	from_lowerTier[i] --> mDHTModule.from_lowerTier[i];
        	to_lowerTier[i] <-- mDHTModule.to_lowerTier[i];
        }
        from_upperTier --> mDHTModule.from_upperTier;
        to_upperTier <-- mDHTModule.to_upperTier;
        trace_in --> mDHTModule.trace_in;
}
