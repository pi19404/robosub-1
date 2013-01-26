/* Image Recognition Knowledge Base Class */

#include "image_kb.h"

IMAGE_KB::IMAGE_KB()
{
    // Start Gate
    Pillar1Seen = false;
    Pillar2Seen = false;

    // Paths
    Path1Seen = false;
    Path2Seen = false;
    Path3Seen = false;
    Path4Seen = false;
    Path5Seen = false;
    Path6Seen = false;
    Path7Seen = false;
    twoPaths = false;

    // Buoys
    BuoyGreenSeen = false;
    BuoyRedSeen = false;
    BuoyYellowSeen = false;

    // Obstacle Course
    BarSeen = false;

    // Torpedoes
    PrimaryTorpedoTargetSeen = false;
    SecondaryTorpedoTargetSeen = false;

    // Bins
    BinsSeen = false;
    swordSeen = false;
    shieldSeen = false;
    netSeen = false;
    tridentSeen = false;
    PrimaryBinTargetSeen = false;
    SecondaryBinTargetSeen = false;

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
