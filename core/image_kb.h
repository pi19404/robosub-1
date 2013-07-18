/* Image Recognition Knowledge Base Class */

#ifndef __IMAGE_KB__
#define __IMAGE_KB__

#include "types.h"

typedef struct BUOY {
	bool buoySeen;
	int buoyX;
	int buoyY;
	int buoyZ;
	colors buoyColor;
}Buoy;

typedef struct TORPEDO_TARGET {
	bool targetSeen;
	int targetX;
	int targetY;
	int targetZ;
	colors targetColor;
}TorpedoTarget;

typedef struct BIN {
	bool binSeen;
	int binX;
	int binY;
	int binZ;
	images binImage;
}Bin;

typedef struct OCTAGON {
	bool octagonSeen;
	int octagonX;
	int octagonY;
	int octagonZ;
}Octagon;

typedef struct SGPILLAR {
	bool pillarSeen;
	int pillarX;
}SGPillar;

class IMAGE_KB {
    public:

        void initializeIM(IMAGE_KB *im);
        IMAGE_KB();

        // Mission Plan
        colors buoyPrimary;
        colors torpedoPrimary;
        colors torpedoSecondary;
        images binPrimary;
        images binSecondary;

        // Current Obstacle
        bool gateObstacle;
        bool pathObstacle;
        bool buoyObstacle;
        bool parkingObstacle;
        bool binsObstacle;
        bool torpedoObstacle;
        bool drivingObstacle;
        bool sonarObstacle;
        
        // Start Gate
		SGPillar sgPillars[2];
		float startGateCenterDistance;
//        bool pillar1Seen, pillar2Seen;
//        int  pillar1X, pillar2X;

        // Paths
        bool pathSeen;
        //bool twoPaths;
        //int  leftPathX;
        int  rightPathX;
		//double  leftPathHeading;
		double  rightPathHeading;

        // Buoys
		Buoy buoys[3];
		int numBuoysSeen;
//        bool buoy1Seen, buoy2Seen, buoy3Seen;
//        int  buoy1X, buoy1Y, buoy1Z, buoy2X, buoy2Y, buoy2Z, buoy3X, buoy3Y, buoy3Z;
//        colors buoy1Color, buoy2Color, buoy3Color;

        // Parking Obastacle
        bool horizBarSeen;
        bool leftBarSeen;
        bool rightBarSeen;

        int  horizBarX; // midpoint
        int  horizBarY;
        int  horizBarZ;
        int  leftBarX; // stop light
        int  leftBarY; // bottom of bar
        int  leftBarZ;
        int  rightBarX;
        int  rightBarY; // bottom of bar
        int  rightBarZ;

        colors leftBarColor;
        colors rightBarColor;

        // Torpedoes
        //bool primaryTorpedoTargetSeen;
        //bool secondaryTorpedoTargetSeen;
		TorpedoTarget torpedoTargets[4];
//        bool redTargetSeen, greenTargetSeen, purpleTargetSeen, blueTargetSeen;
//        int  redTargetX,redTargetY, redTargetZ,greenTargetX, greenTargetY,greenTargetZ,purpleTargetX, purpleTargetY,purpleTargetZ, blueTargetX,blueTargetY,blueTargetZ;

        // Bins
		Bin bins[4];
//        bool swordSeen, shieldSeen, netSeen, tridentSeen;
//        int  swordX, swordY, swordZ, shieldX, shieldY, shieldZ, netX, netY, netZ, tridentX, tridentY, tridentZ;

        // Manipulation
        bool wheelSeen;
        bool shifterSeen;
        int  wheelRot;
        int  wheelX;
        int  wheelY;
        int  wheelZ;
        int  shiftX;
        int  shiftY;
        int  shiftZ;

        // Octagons
		Octagon octagons[1];
//        bool objectSeen;
//		  object to pick up below the octagon
//        int  objectX, objectY, objectZ;
	
        // Center point for target one
        int x1;  // pixels? 
        int y1;
        int z1;
        int heading1; // degrees
        colors target1color;
        images target1image;

        // Center point for target two 
        int x2;
        int y2;
        int z2;
        int heading2;
        colors target2color;
        images target2image;
        
        // Center point for target three
        int x3;
        int y3;
        int z3;
        int heading3;
        colors target3color;
        images target3image;

        // Center point for target four
        int x4;
        int y4;
        int z4;
        int heading4;
        colors target4color;
        images target4image;

    private:


};


#endif
