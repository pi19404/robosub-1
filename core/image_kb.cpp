/* Image Recognition Knowledge Base Class */

#include "image_kb.h"

IMAGE_KB::IMAGE_KB()
{
    // Start Gate
    pillar1Seen = false;
    pillar2Seen = false;
    pillar1X    = 0;
    pillar2X    = 0;

    // Paths
    path1Seen  = false;
    path2Seen  = false;
    path3Seen  = false;
    path4Seen  = false;
    path5Seen  = false;
    path6Seen  = false;
    path7Seen  = false;
    twoPaths   = false;
    leftPathX  = 0;
    rightPathX = 0;

    // Buoys
    buoy1Seen  = false;
    buoy2Seen  = false;
    buoy3Seen  = false;
    buoy1X     = 0;
    buoy1Y     = 0;
    buoy1Z     = 0;
    buoy2X     = 0;
    buoy2Y     = 0;
    buoy2Z     = 0;
    buoy3X     = 0;
    buoy3Y     = 0;
    buoy3Z     = 0;
    buoy1Color = DEFAULT;
    buoy2Color = DEFAULT;
    buoy3Color = DEFAULT;

    // Parking Obastacle
    horizBarSeen = false;
    leftBarSeen  = false;
    rightBarSeen = false;
    
    horizBarX = 0; // midpoint
    horizBarY = 0;
    horizBarZ = 0;
    leftBarX  = 0; // stop light
    leftBarY  = 0; // bottom of bar
    leftBarZ  = 0;
    rightBarX = 0;
    rightBarY = 0; // bottom of bar
    rightBarZ = 0;

    // Torpedoes
    redTargetSeen    = false;
    greenTargetSeen  = false;
    purpleTargetSeen = false;
    blueTargetSeen   = false;

    redTargetX    = 0;
    redTargetY    = 0;
    redTargetZ    = 0;
    greenTargetX  = 0;
    greenTargetY  = 0;
    greenTargetZ  = 0;
    purpleTargetX = 0;
    purpleTargetY = 0;
    purpleTargetZ = 0;
    blueTargetX   = 0;
    blueTargetY   = 0;
    blueTargetZ   = 0;

    // Bins
    swordSeen   = false;
    shieldSeen  = false;
    netSeen     = false;
    tridentSeen = false;
    
    swordX   = 0;
    swordY   = 0;
    swordZ   = 0;
    shieldX  = 0;
    shieldY  = 0;
    shieldZ  = 0;
    netX     = 0;
    netY     = 0;
    netZ     = 0;
    tridentX = 0;
    tridentY = 0;
    tridentZ = 0;

    // Manipulation
    wheelSeen   = false;
    shifterSeen = false;
    wheelRot    = false;
    wheelX      = 0;
    wheelY      = 0;
    wheelZ      = 0;
    shiftX      = 0;
    shiftY      = 0;
    shiftZ      = 0;

    // Octagons
    objectSeen = false;
    objectX    = 0;  // object to pick up below the octagon
    objectY    = 0;
    objectZ    = 0;

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
