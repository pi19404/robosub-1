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
        bool Pillar1Seen;
        bool Pillar2Seen;

        // Paths
        bool Path1Seen;
        bool Path2Seen;
        bool Path3Seen;
        bool Path4Seen;
        bool Path5Seen;
        bool Path6Seen;
        bool Path7Seen;
        bool twoPaths;

        // Buoys
        bool BuoyGreenSeen;
        bool BuoyRedSeen;
        bool BuoyYellowSeen;

        // Obstacle Course
        bool BarSeen;


        // Torpedoes
        bool PrimaryTorpedoTargetSeen;
        bool SecondaryTorpedoTargetSeen;

        // Bins
        bool BinsSeen;
        bool swordSeen;
        bool shieldSeen;
        bool netSeen;
        bool tridentSeen;
        bool PrimaryBinTargetSeen;
        bool SecondaryBinTargetSeen;

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
