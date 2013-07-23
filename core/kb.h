// Knowledge Base Class */

#ifndef __KB__
#define __KB__

class IMAGE_KB;

#include "types.h"
#include <iostream>
#include <fstream>

class KB {

    public:
        KB(); // Constructor
        int updateKB(IMAGE_KB *im); 
        void printKB(KB *kb);
	void updatePillars(IMAGE_KB *im);
	void updateBuoys(IMAGE_KB *im);
	void updateBins(IMAGE_KB *im);
	void updateTorpedos(IMAGE_KB *im);
	void updatePaths(IMAGE_KB *im);

        // Multiple Tasks
        bool startGateComplete;
        bool buoyTaskComplete;
        bool obstacleCourse1Complete;
        bool torpedoTaskComplete;
        bool binsTaskComplete;
        bool obstacleCourse2Complete;

        bool targetInRange; // Torpedos, Bins
        bool attemptTask;

        // Start Gate  // Used for Obstacle Course Too
        bool pillar1Found;
        bool pillar2Found;

        // Paths
        //bool path1found;
        //bool path2found;
        //bool path3found;
        //bool path4found;
        //bool path5found;
        //bool path6found;
        //bool path8found;
        bool pathfound[8];
        

        // Buoys
        bool correctHeading;
        //bool buoy1Found;
        //bool buoy2Found;
        //bool buoy3Found;
        bool buoyFound[3];
        //bool buoy1Complete;
        //bool buoy2Complete;
        //bool buoy3Complete;
        bool buoyComplete[3];
        //colors buoy1Color;
        //colors buoy2Color;
        //colors buoy3Color;
        colors buoyColor[3];
	//bool buoy1Hit;
	//bool buoy2Hit;
	//bool buoy3Hit;
	bool buoyHit[3];
        
        // Parking Obstacle
        bool horizBarFound;
        bool leftBarFound;
        bool rightBarFound;

        // re use gate pillar booleans

        // Torpedoes
        bool primaryTorpedoTargetFound;
	bool secondaryTorpedoTargetFound;
        bool primaryTorpedoTargetComplete;
        bool secondaryTorpedoTargetComplete;
        int  torpedoDistance;

        // Bins
        bool binsFound;
        bool binsPrimaryFound;
        bool binsSecondaryFound;
        bool primaryBinTargetComplete;
        bool secondaryBinTargetComplete;
        int  binDistance;

        // Octaons
        bool objectFound;
        bool objectGrabbed;
        int  objectDistance;
        int  pingerDistance;
        int  pingerHeading; 
         
        

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
		colors buoyGoalColor;
        
        // Torpedo Targets
        colors torpedoPrimary;
        colors torpedoSecondary;

        // Bin Targets
        images binPrimary;
        images binSecondary;

    private:

};

#endif
