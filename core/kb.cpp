/* Knowledge Base cpp */

#include "kb.h"
#include "image_kb.h"

KB::KB()
{
        // Multiple Tasks
        startGateComplete = false;
        buoyTaskComplete = false;
        obstacleCourse1Complete = false;
        torpedoTaskComplete = false;
        binsTaskComplete = false;
        obstacleCourse2Complete = false;

        targetInRange = false; // Torpedos, Bins
        attemptTask = false;

        // Start Gate  // Used for Obstacle Course Too
        pillar1Found = false;
        pillar2Found = false;

        // Paths
        path1found = false;
        path2found = false;
        path3found = false;
        path4found = false;
        path5found = false;
        path6found = false;
        path7found = false;

        // Buoys
        correctHeading = false;
        buoy1Found = false;
        buoy2Found = false;
        buoy3Found = false;
        buoy1Complete = false;
        buoy2Complete = false;
        buoy3Complete = false;
        buoy1Color = DEFAULT;
        buoy2Color = DEFAULT; 
        buoy3Color = DEFAULT;
        buoy1Hit   = false;
        buoy2Hit   = false;
        buoy3Hit   = false;
        
        // Parking Obstacle
        horizBarFound = false;
        leftBarFound = false;
        rightBarFound = false;

        // re use gate pillar booleans

        // Torpedoes
        primaryTorpedoTargetComplete = false;
        secondaryTorpedoTargetComplete = false;
        torpedoDistance = 0;

        // Bins
        binsFound  = false;
        binsPrimaryFound = false;
        binsSecondaryFound = false;
        primaryBinTargetComplete = false;
        secondaryBinTargetComplete = false;
        binDistance = false;

        // Octaons
        objectFound = false;
        objectGrabbed = false;
        objectDistance = 0;
        pingerDistance = 0;
        pingerHeading = 0; 



        // Positioning Variables
        // Center point for target one
        x1 = 0;  // pixels? 
        y1 = 0;
        z1 = 0;
        heading1 = 0; // degrees

        // Center point for target two 
        x2 = 0;
        y2 = 0;
        z2 = 0;
        heading2 = 0;
        
        // Center point for target three
        x3 = 0;
        y3 = 0;
        z3 = 0;
        heading3 = 0;

        // Center point for target three
        x4 = 0;
        y4 = 0;
        z4 = 0;
        heading4 = 0;

        // current depth and min depth for traveling
        depth = 0;
        minDepth = 1;

        // If needed can add other variable for storage
            // buoys and bins might need more
        
        // TODO LIVE MISSION PLAN UPDATE
        // Buoy Targets
        buoy1Complete = false;
        buoy1Found = false;
        buoy2Complete = false;
        buoy2Found = false;
        buoy3Complete = false;
        buoy3Found = false;
        buoyGoalColor = DEFAULT;	// Either: set this if we know what color we want
											// to make the buoys or set it when we collide
											// with the first buoy so the rest of the buoys match
        
        // Torpedo Targets
        torpedoPrimary   = DEFAULT;
        torpedoSecondary = DEFAULT;

        // Bin Targets
        binPrimary   = UNKNOWN;
        binSecondary = UNKNOWN;

//        return this;
}

// update KB found based on image recognition
int KB::updateKB(IMAGE_KB *im)
{
	updatePillars(im);
	updateBuoys(im);
	updateBins(im);
	updateTorpedos(im);
	return 1; // should it return this or something else?
}

void KB::updatePillars(IMAGE_KB *im)
{
    // update found
    if(im->sgPillars[0].pillarSeen)
        pillar1Found = true;
    if(im->sgPillars[1].pillarSeen)
        pillar2Found = true;
}

void KB::updateBuoys(IMAGE_KB *im)
{
    int i = 0;
	for(i = 0; i < 3; i++)
	{
/*		if(im->buoy[i].buoySeen)
		{
			
		}*/	
	}	
}


void KB::updateBins(IMAGE_KB *im)
{
	int flag = 0;
	int count = 0;
    int i = 0;

	for(i = 0; i < 4; i++)
	{
		count += 1;

		if(im->bins[i].binSeen)
		{
			if(im->bins[i].binImage == binPrimary)
			{
				primaryTorpedoTargetFound = true;
				x1 = im->bins[i].binX;
				y1 = im->bins[i].binY;
				z1 = im->bins[i].binZ;
			}
			else if(im->bins[i].binImage == binSecondary)
			{
				secondaryTorpedoTargetFound = true;
				x2 = im->bins[i].binX;
				y2 = im->bins[i].binY;
				z2 = im->bins[i].binZ;
			}
			else if(flag == 0)
			{
				flag = 1;
				x3 = im->bins[i].binX;
				y3 = im->bins[i].binY;
				z3 = im->bins[i].binZ;
			}
			else
			{
				x4 = im->bins[i].binX;
				y4 = im->bins[i].binY;
				z4 = im->bins[i].binZ;
			}
		}
	}
	
	if(count == 4)
	{
		binsFound = true;
	}
}

void KB::updateTorpedos(IMAGE_KB *im)
{
	int flag = 0;
    int i = 0;

	for(i = 0; i < 4; i++)
	{
		if(im->torpedoTargets[i].targetSeen)
		{
			if(im->torpedoTargets[i].targetColor == torpedoPrimary)
			{
				primaryTorpedoTargetFound = true;
				x1 = im->torpedoTargets[i].targetX;
				y1 = im->torpedoTargets[i].targetY;
				z1 = im->torpedoTargets[i].targetZ;
			}
			else if(im->torpedoTargets[i].targetColor == torpedoSecondary)
			{
				secondaryTorpedoTargetFound = true;
				x2 = im->torpedoTargets[i].targetX;
				y2 = im->torpedoTargets[i].targetY;
				z2 = im->torpedoTargets[i].targetZ;
			}
			else if(flag == 0)
			{
				flag = 1;
				x3 = im->torpedoTargets[i].targetX;
				y3 = im->torpedoTargets[i].targetY;
				z3 = im->torpedoTargets[i].targetZ;
			}
			else
			{
				x4 = im->torpedoTargets[i].targetX;
				y4 = im->torpedoTargets[i].targetY;
				z4 = im->torpedoTargets[i].targetZ;
			}
		}
	}
}


void KB::printKB(KB *kb)
{
    //file = fopen("kb.txt", 'w');
    fstream file;
    file.open("kb.txt");

    file << "Start Gate Complete: " << startGateComplete << endl;
    file << "Buoy Task Complete:  " << buoyTaskComplete << endl; 
    file << "Obstacle Course 1 Complete: " << obstacleCourse1Complete << endl;
    file << "Torpedo Task Complete: " << torpedoTaskComplete << endl;
    file << "Bins Task Complete: " << binsTaskComplete << endl;
    file << "Obstacle Course 2 Complete: " << obstacleCourse2Complete << endl;
    file << "Target In Range: " << targetInRange << endl;
    file << "Attempt Task: " << attemptTask << endl;
    file << "pillar1Found: " << pillar1Found << endl;
    file << "pillar2Found: " << pillar2Found << endl;
    file << "path1found: " << path1found << endl;
    file << "path2found: " << path2found << endl;
    file << "path3found: " << path3found << endl;
    file << "path4found: " << path4found << endl;
    file << "path5found: " << path5found << endl;
    file << "path6found: " << path6found << endl;
    file << "path7found: " << path7found << endl;
    file << "correctHeading: " << correctHeading << endl;
    file << "buoy1Found: " << buoy1Found << endl;
    file << "buoy2Found: " << buoy2Found << endl;
    file << "buoy3Found: " << buoy3Found << endl;
    file << "buoy1Complete: " << buoy1Complete << endl;
    file << "buoy2Complete: " << buoy2Complete << endl;
    file << "buoy3Complete: " << buoy3Complete << endl;
    file << "buoy1Color: " << buoy1Color << endl;
    file << "buoy2Color: " << buoy2Color << endl;
    file << "buoy3Color: " << buoy3Color << endl;
    file << "buoy1Hit: " << buoy1Hit << endl;
    file << "buoy2Hit: " << buoy2Hit << endl;
    file << "buoy3Hit: " << buoy3Hit << endl;
    file << "horizBarFound: " << horizBarFound << endl;
    file << "leftBarFound: " << leftBarFound << endl;
    file << "rightBarFound: " << rightBarFound << endl;
    file << "primaryTorpedoTargetComplete: " << primaryTorpedoTargetComplete << endl;
    file << "secondaryTorpedoTargetComplete: " << secondaryTorpedoTargetComplete << endl;
    file << "torpedoDistance: " << torpedoDistance << endl;
    file << "binsFound: " << binsFound << endl;
    file << "binsPrimaryFound: " << binsPrimaryFound << endl; 
    file << "binsSecondaryFound: " << binsSecondaryFound << endl;
    file << "primaryBinTargetComplete: " << primaryBinTargetComplete << endl;
    file << "secondaryBinTargetComplete: " << secondaryBinTargetComplete << endl;
    file << "binDistance: " << binDistance << endl;
    file << "objectFound: " << objectFound << endl;
    file << "objectGrabbed: " << objectGrabbed << endl;
    file << "objectDistance: " << objectDistance << endl;
    file << "pingerDistance: " << pingerDistance << endl;
    file << "pingerHeading: " << pingerHeading << endl;
    file << "x1: " << x1 << endl;
    file << "y1: " << y1 << endl;
    file << "z1: " << z1 << endl;
    file << "heading1: " << heading1 << endl;
    file << "x2: " << x2 << endl;
    file << "y2: " << y2 << endl;
    file << "z2: " << z2 << endl;
    file << "heading2: " << heading2 << endl;
    file << "x3: " << x3 << endl;
    file << "y3: " << y3 << endl;
    file << "z3: " << z3 << endl;
    file << "heading3: " << heading3 << endl;
    file << "x4: " << x4 << endl;
    file << "y4: " << y4 << endl;
    file << "z4: " << z4 << endl;
    file << "heading4: " << heading4 << endl;

    file << "depth: " << depth << endl;
    file << "minDepth: " << minDepth << endl;

    file << "buoy1Complete: " << buoy1Complete << endl;
    file << "buoy1Found: " << buoy1Found << endl;
    file << "buoy2Complete: " << buoy2Complete << endl;
    file << "buoy2Found: " << buoy2Found << endl;
    file << "buoy3Complete: " << buoy3Complete << endl;


        // If needed can add other variable for storage
            // buoys and bins might need more
        
        // TODO LIVE MISSION PLAN UPDATE
        // Buoy Targets
		buoy1Complete = false;
		buoy1Found = false;
		buoy2Complete = false;
		buoy2Found = false;
		buoy3Complete = false;
		buoy3Found = false;
		buoyGoalColor = DEFAULT;	// Either: set this if we know what color we want
											// to make the buoys or set it when we collide
											// with the first buoy so the rest of the buoys match
        
        // Torpedo Targets
        torpedoPrimary   = DEFAULT;
        torpedoSecondary = DEFAULT;

        // Bin Targets
        binPrimary   = UNKNOWN;
        binSecondary = UNKNOWN;
        file.close();
}

