package oversim.applications.scribetest;

import oversim.common.BaseApp;
import oversim.common.ITier;


simple ScribeTest extends BaseApp
{
    parameters:
        @class(ScribeTest);
}

module ScribeTestModule like ITier
{
    gates:
        input udpIn;
        input from_lowerTier;
        input from_upperTier;
        output udpOut;
        output to_lowerTier;
        output to_upperTier;

    submodules:
        scribeTest: ScribeTest;
    connections allowunconnected:
        from_lowerTier --> scribeTest.from_lowerTier;
        to_lowerTier <-- scribeTest.to_lowerTier;
        from_upperTier --> scribeTest.from_upperTier;
        to_upperTier <-- scribeTest.to_upperTier;
        udpIn --> scribeTest.udpIn;
        udpOut <-- scribeTest.udpOut;

}

