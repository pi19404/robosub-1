/* Knowledge Base Class */

#ifndef __KB__
#define __KB__

class IMAGE_KB;

namespace colors
{
    enum colors {DEFAULT, RED, BLUE, YELLOW, GREEN}; 
}
namespace images
{

    enum images {UNKNOWN, SWORD, TRIDENT, SHIELD, NET};
}

class KB {

    public:
        KB(); // Constructor
        int updateKB(IMAGE_KB *im); 

        // Multiple Tasks
        bool StartGateComplete;
        bool BuoyTaskComplete;
        bool ObstacleCourse1Complete;
        bool TorpedoTaskComplete;
        bool BinsTaskComplete;
        bool ObstacleCourse2Complete;

        bool TargetInRange; // Torpedos, Bins
        bool AttemptTask;

        // Start Gate  // Used for Obstacle Course Too
        bool Pillar1Found;
        bool Pillar2Found;

        // Paths


        // Buoys
        bool CorrectHeading;
        bool buoyPrimaryFound;
        bool buoySecondaryFound;
        bool PrimaryBuoyComplete;
        bool SecondaryBuoyComplete;
        
        // Obstacle Course
        bool BarFound;

        // re use gate pillar booleans

        // Torpedoes
        bool PrimaryTorpedoTargetComplete;
        bool SecondaryTorpedoTargetComplete;

        // Bins
        bool BinsFound;
        bool BinsPrimaryFound;
        bool BinsSecondaryFound;
        bool PrimaryBinTargetComplete;
        bool SecondaryBinTargetComplete;
        bool DistancesCalculated;

        // Positioning Variables
        // Center point for primary target

        int x1;  // pixels? 
        int y1;
        int z1;
        int heading1; // degrees

        // Center point for secondary target 
        int x2;
        int y2;
        int z2;
        int heading2;

        // Center point for third target 
        int x3;
        int y3;
        int z3;
        int heading3;

        // Center point for fourth target 
        int x4;
        int y4;
        int z4;
        int heading4;

        // current depth and min depth for traveling
        int depth;
        int minDepth;
        
        // Predefined at competition time
        // Buoy Targets
        colors::colors buoyPrimary;
        colors::colors buoySecondary;
        
        // Torpedo Targets
        colors::colors torpedoPrimary;
        colors::colors torpedoSecondary;

        // Bin Targets
        images::images binPrimary;
        images::images binSecondary;

    private:

};

#endif
