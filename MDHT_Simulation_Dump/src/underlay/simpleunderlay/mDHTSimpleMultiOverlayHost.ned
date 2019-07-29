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


package oversim.underlay.simpleunderlay;

import inet.base.NotificationBoard;
import inet.networklayer.common.InterfaceTable;
import oversim.common.BootstrapList;
import oversim.common.CryptoModule;
import oversim.common.IOverlay;
import oversim.common.ITier;
import oversim.common.NeighborCache;
import oversim.applications.mDHT.mDHTModules;
import oversim.common.TierDummy;


//
// Host in the simple network that participates in the overlay
//
// @author Stephan Krause, Bernhard Heep, Ingmar Baumgart
//
module mDHTSimpleMultiOverlayHost
{
    parameters:
        string overlayType; // overlay protocol compound module to use        
        string tier1Type; // tier 1 application to use
        string tier2Type;// tier 2 module to use
        string tier3Type; // tier 3 module to use
        int numTiers; // number of tiers
        double numLevels; // number of parallel overlays
        string routingFile = default("");
        bool IPForward = default(false);

        @display("bgb=433,386");
    gates:
        input overlayNeighborArrowIn[]; // incoming gate for visualizing overlay neighborship with connection arrows
        output overlayNeighborArrowOut[]; // incoming gate for visualizing overlay neighborship with connection arrows
        output out; // Dummy gate for storing channel informations
        input in; // Dummy gate for connecting out

    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=56,224");
        }
        tier3: <tier3Type> like ITier {
            parameters:
                @display("p=64,64;i=block/segm");
        }
        tier2: <tier2Type> like ITier {
            parameters:
                @display("p=64,64;i=block/segm");
        }
        tier1: mDHTModules {
            parameters:
                @display("p=179,104;i=block/segm");
        }
        overlay[numLevels]: <overlayType> like IOverlay {
            parameters:
                @display("p=370,216;i=block/network2");
        }
        udp: SimpleUDP {
            parameters:
                @display("p=370,342");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=64,287");
        }
        neighborCache: NeighborCache {
            parameters:
                @display("p=168,287");
        }
        cryptoModule: CryptoModule {
            parameters:
                @display("p=56,162");
        }
        bootstrapList: BootstrapList {
            parameters:
                @display("p=168,224");

        }

    connections allowunconnected:
        bootstrapList.udpOut --> udp.appIn++;
        bootstrapList.udpIn <-- udp.appOut++;

        for i=0..numLevels-1 {
                tier1.to_lowerTier[i] --> overlay[i].appIn if numTiers>0;
                tier1.from_lowerTier[i] <-- overlay[i].appOut if numTiers>0;
                
                overlay[i].udpOut --> udp.appIn++;
                overlay[i].udpIn <-- udp.appOut++;
                
        }
        
        tier1.udpOut --> udp.appIn++ if numTiers>0;
        udp.appOut++ --> tier1.udpIn if numTiers>0;

        tier2.to_lowerTier --> tier1.from_upperTier if numTiers > 1;
        tier2.from_lowerTier <-- tier1.to_upperTier if numTiers > 1;
        tier2.udpOut --> udp.appIn++ if numTiers>1;
        udp.appOut++ --> tier2.udpIn if numTiers>1;

        tier3.to_lowerTier --> tier2.from_upperTier if numTiers > 2;
        tier3.from_lowerTier <-- tier2.to_upperTier if numTiers > 2;
        tier3.udpOut --> udp.appIn++ if numTiers>2;
        udp.appOut++ --> tier3.udpIn if numTiers>2;

}

