/* Image Recognition Knowledge Base Class */

#include "image_kb.h"
void initializeIM(IMAGE_KB *im)
{
    // Current Obstacle
    gateObstacle    = false;
    pathObstacle    = false;
    buoyObstacle    = false;
    parkingObstacle = false;
    binsObstacle    = false;
    torpedoObstacle = false;
    drivingObstacle = false;
    sonarObstacle   = false;

	// Start Gate
	im->startGateCenterDistance = -1.0;
    im->sgPillars[0].pillarSeen = false;
    im->sgPillars[1].pillarSeen = false;
    im->sgPillars[0].pillarX    = 0;
    im->sgPillars[1].pillarX    = 0;

    // Paths
    im->path1Seen        = false;
    im->path2Seen        = false;
    im->path3Seen        = false;
    im->path4Seen        = false;
    im->path5Seen        = false;
    im->path6Seen        = false;
    im->path7Seen        = false;
    im->twoPaths         = false;
    im->leftPathX        = 0;
    im->rightPathX       = 0; // if only one path use the right path variable
    im->leftPathHeading  = 0;
    im->rightPathHeading = 0; // if only one path use the right path variable


    // Buoys
	im->numBuoysSeen = 0;
    im->buoys[0].buoySeen  = false;
    im->buoys[1].buoySeen  = false;
    im->buoys[2].buoySeen  = false;
    im->buoys[0].buoyX     = 0;
    im->buoys[1].buoyY     = 0;
    im->buoys[2].buoyZ     = 0;
    im->buoys[0].buoyX     = 0;
    im->buoys[1].buoyY     = 0;
    im->buoys[2].buoyZ     = 0;
    im->buoys[0].buoyX     = 0;
    im->buoys[1].buoyY     = 0;
    im->buoys[2].buoyZ     = 0;
    im->buoys[0].buoyColor = DEFAULT;
    im->buoys[1].buoyColor = DEFAULT;
    im->buoys[2].buoyColor = DEFAULT;

    // Parking Obastacle
    im->horizBarSeen = false;
    im->leftBarSeen  = false;
    im->rightBarSeen = false;
    
    im->horizBarX = 0; // midpoint
    im->horizBarY = 0;
    im->horizBarZ = 0;
    im->leftBarX  = 0; // stop light
    im->leftBarY  = 0; // bottom of bar
    im->leftBarZ  = 0;
    im->rightBarX = 0;
    im->rightBarY = 0; // bottom of bar
    im->rightBarZ = 0;

    // Torpedoes
    im->torpedoTargets[0].targetSeen = false;
    im->torpedoTargets[1].targetSeen = false;
    im->torpedoTargets[2].targetSeen = false;
    im->torpedoTargets[3].targetSeen = false;

    im->torpedoTargets[0].targetX = 0;
    im->torpedoTargets[0].targetY = 0;
    im->torpedoTargets[0].targetZ = 0;
    im->torpedoTargets[1].targetX = 0;
    im->torpedoTargets[1].targetY = 0;
    im->torpedoTargets[1].targetZ = 0;
    im->torpedoTargets[2].targetX = 0;
    im->torpedoTargets[2].targetY = 0;
    im->torpedoTargets[2].targetZ = 0;
    im->torpedoTargets[3].targetX = 0;
    im->torpedoTargets[3].targetY = 0;
    im->torpedoTargets[3].targetZ = 0;

    // Bins
    im->bins[0].binSeen = false;
	im->bins[1].binSeen = false;
	im->bins[2].binSeen = false;
	im->bins[3].binSeen = false;
    
    im->bins[0].binX = 0;
	im->bins[0].binY = 0;
	im->bins[0].binZ = 0;
	im->bins[1].binX = 0;
	im->bins[1].binY = 0;
	im->bins[1].binZ = 0;
	im->bins[2].binX = 0;
	im->bins[2].binY = 0;
	im->bins[2].binZ = 0;
	im->bins[3].binX = 0;
	im->bins[3].binY = 0;
	im->bins[3].binZ = 0;

    // Manipulation
    im->wheelSeen   = false;
    im->shifterSeen = false;
    im->wheelRot    = false;
    im->wheelX      = 0;
    im->wheelY      = 0;
    im->wheelZ      = 0;
    im->shiftX      = 0;
    im->shiftY      = 0;
    im->shiftZ      = 0;

    // Octagons
	im->octagons[0].octagonSeen = false;
	im->octagons[0].octagonX    = 0;	// object to pick up below the octagon
	im->octagons[0].octagonY    = 0;
	im->octagons[0].octagonZ    = 0;

    // Center point for target one
    im->x1 = 0;  // pixels? 
    im->y1 = 0;
    im->z1 = 0;
    im->heading1 = 0; // degrees
    im->target1color = DEFAULT;
    im->target1image = UNKNOWN;

    // Center point for target two 
    im->x2 = 0;
    im->y2 = 0;
    im->z2 = 0;
    im->heading2 = 0;
    im->target2color = DEFAULT;
    im->target2image = UNKNOWN;
    
    // Center point for target three
    im->x3 = 0;
    im->y3 = 0;
    im->z3 = 0;
    im->heading3 = 0;
    im->target3color = DEFAULT;
    im->target3image = UNKNOWN;

    // Center point for target four
    im->x4 = 0;
    im->y4 = 0;
    im->z4 = 0;
    im->heading4;
    im->target4color = DEFAULT;
    im->target4image = UNKNOWN;

	return;
}

IMAGE_KB::IMAGE_KB()
{
	initializeIM(this);
}
