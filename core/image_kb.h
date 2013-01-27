/* Image Recognition Knowledge Base Class */

#ifndef __IMAGE_KB__
#define __IMAGE_KB__

class KB;

class IMAGE_KB {
    public:

        IMAGE_KB();
        enum colors {DEFAULT, RED, BLUE, YELLOW, GREEN}; 
        enum images {UNKNOWN, SWORD, TRIDENT, SHIELD, NET};

        // Start Gate
        bool pillar1Seen;
        bool pillar2Seen;
        int  pillar1X;
        int  pillar2X;

        // Paths
        bool path1Seen;
        bool path2Seen;
        bool path3Seen;
        bool path4Seen;
        bool path5Seen;
        bool path6Seen;
        bool path7Seen;
        bool twoPaths;
        int  leftPathX;
        int  rightPathX;

        // Buoys
        bool buoyGreenSeen;
        bool buoyRedSeen;
        bool buoyYellowSeen;
        int  buoyGreenX;
        int  buoyGreenY;
        int  buoyGreenZ;
        int  buoyRedX;
        int  buoyRedY;
        int  buoyRedZ;
        int  buoyYellowX;
        int  buoyYellowY;
        int  buoyYellowZ;

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

        // Torpedoes
        //bool primaryTorpedoTargetSeen;
        //bool secondaryTorpedoTargetSeen;
        bool redTargetSeen;
        bool greenTargetSeen;
        bool purpleTargetSeen;
        bool blueTargetSeen;

        int  redTargetX;
        int  redTargetY;
        int  redTargetZ;
        int  greenTargetX;
        int  greenTargetY;
        int  greenTargetZ;
        int  purpleTargetX;
        int  purpleTargetY;
        int  purpleTargetZ;
        int  blueTargetX;
        int  blueTargetY;
        int  blueTargetZ;

        // Bins
        //bool binsSeen;
        bool swordSeen;
        bool shieldSeen;
        bool netSeen;
        bool tridentSeen;
        //bool primaryBinTargetSeen;
        //bool secondaryBinTargetSeen;
        int  swordX;
        int  swordY;
        int  swordZ;
        int  shieldX;
        int  shieldY;
        int  shieldZ;
        int  netX;
        int  netY;
        int  netZ;
        int  tridentX;
        int  tridentY;
        int  tridentZ;

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
        bool objectSeen;
        int  objectX;  // object to pick up below the octagon
        int  objectY;
        int  objectZ;
	
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
