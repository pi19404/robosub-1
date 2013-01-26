/* Image Recognition Knowledge Base Class */

#include "image_kb.h"

IMAGE_KB::IMAGE_KB()
{
    // Start Gate
    pillar1Seen = false;
    pillar2Seen = false;

    // Paths
    path1Seen = false;
    path2Seen = false;
    path3Seen = false;
    path4Seen = false;
    path5Seen = false;
    path6Seen = false;
    path7Seen = false;
    twoPaths = false;

    // Buoys
    buoyGreenSeen = false;
    buoyRedSeen = false;
    buoyYellowSeen = false;

    // Obstacle Course
    barSeen = false;

    // Torpedoes
    primaryTorpedoTargetSeen = false;
    secondaryTorpedoTargetSeen = false;

    // Bins
    binsSeen = false;
    swordSeen = false;
    shieldSeen = false;
    netSeen = false;
    tridentSeen = false;
    primaryBinTargetSeen = false;
    secondaryBinTargetSeen = false;

    // Center point for target one
    x1 = 0;  // pixels? 
    y1 = 0;
    z1 = 0;
    heading1 = 0; // degrees
    target1color = DEFAULT;
    target1image = UNKNOWN;

    // Center point for target two 
    x2 = 0;
    y2 = 0;
    z2 = 0;
    heading2 = 0;
    target2color = DEFAULT;
    target2image = UNKNOWN;
    
    // Center point for target three
    x3 = 0;
    y3 = 0;
    z3 = 0;
    heading3 = 0;
    target3color = DEFAULT;
    target3image = UNKNOWN;

    // Center point for target four
    x4 = 0;
    y4 = 0;
    z4 = 0;
    heading4;
    target4color = DEFAULT;
    target4image = UNKNOWN;
};
