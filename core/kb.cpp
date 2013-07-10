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
        int torpedoDistance = 0;

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
    // update found
    if(im->sgPillars[0].pillarSeen)
        pillar1Found = true;
    if(im->sgPillars[1].pillarSeen)
        pillar2Found = true;
/*
    // Set Primary Buoy Target
    if(im->buoyGreenSeen && buoyPrimary == GREEN)
    {
        buoyPrimaryFound = true;
        if(im->target1color == GREEN)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2color == GREEN)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3color == GREEN)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
    }
    if(im->buoyRedSeen && buoyPrimary == RED)
    {
        buoyPrimaryFound = true;
        if(im->target1color == RED)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2color == RED)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3color == RED)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
    }
    if(im->buoyYellowSeen && buoyPrimary == YELLOW)
    {
        buoyPrimaryFound = true;
        if(im->target1color == YELLOW)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2color == YELLOW)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3color == YELLOW)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
    }
    // Set Secondary Target
    if(kb->buoyPrimaryComplete)
    {
        if(im->buoyGreenSeen && buoySecondary == GREEN)
        {
            buoySecondaryFound = true;
            if(im->target1color == GREEN)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2color == GREEN)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3color == GREEN)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4color == GREEN)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }
        }
        if(im->buoyRedSeen && buoySecondary == RED)
        {
            buoySecondaryFound = true;
            if(im->target1color == RED)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2color == RED)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3color == RED)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4color == RED)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }
        }
        if(im->buoyYellowSeen && buoySecondary == YELLOW)
        {
            buoySecondaryFound = true;
            if(im->target1color == YELLOW)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2color == YELLOW)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3color == YELLOW)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4color == YELLOW)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }
        }
    }
*/
    // Update Bins Found
//    if(im->binsSeen)
//        binsFound = true;

    if(im->bins[0].binSeen/*swordSeen*/ && binPrimary == SWORD)
    {
        binsPrimaryFound = true;

        if(im->target1image == SWORD)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2image == SWORD)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3image == SWORD)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
        if(im->target4image == SWORD)
        {
            x1 = im->x4;
            y1 = im->y4;
            z1 = im->z4;
            heading1 = im->heading4;
        }

    }
    if(im->bins[1].binSeen && binPrimary == SHIELD)
    {
        binsPrimaryFound = true;
        if(im->target1image == SHIELD)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2image == SHIELD)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3image == SHIELD)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
        if(im->target4image == SHIELD)
        {
            x1 = im->x4;
            y1 = im->y4;
            z1 = im->z4;
            heading1 = im->heading4;
        }

    }
    if(im->bins[2].binSeen && binPrimary == NET)
    {
        binsPrimaryFound = true;
        if(im->target1image == NET)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2image == NET)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3image == NET)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
        if(im->target4image == NET)
        {
            x1 = im->x4;
            y1 = im->y4;
            z1 = im->z4;
            heading1 = im->heading4;
        }

    }
    if(im->bins[3].binSeen && binPrimary == TRIDENT)
    {
        binsPrimaryFound = true;
        if(im->target1image == TRIDENT)
        {
            x1 = im->x1;
            y1 = im->y1;
            z1 = im->z1;
            heading1 = im->heading1;
        }
        if(im->target2image == TRIDENT)
        {
            x1 = im->x2;
            y1 = im->y2;
            z1 = im->z2;
            heading1 = im->heading2;
        }
        if(im->target3image == TRIDENT)
        {
            x1 = im->x3;
            y1 = im->y3;
            z1 = im->z3;
            heading1 = im->heading3;
        }
        if(im->target4image == TRIDENT)
        {
            x1 = im->x4;
            y1 = im->y4;
            z1 = im->z4;
            heading1 = im->heading4;
        }

    }


    if(binsPrimaryFound)
    {
        if(im->bins[0].binSeen && binSecondary == SWORD)
        {
            binsSecondaryFound = true;

            if(im->target1image == SWORD)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2image == SWORD)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3image == SWORD)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4image == SWORD)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }

        }
        if(im->bins[1].binSeen && binSecondary == SHIELD)
        {
            binsSecondaryFound = true;
            if(im->target1image == SHIELD)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2image == SHIELD)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3image == SHIELD)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4image == SHIELD)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }

        }
        if(im->bins[2].binSeen && binSecondary == NET)
        {
            binsSecondaryFound = true;
            if(im->target1image == NET)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2image == NET)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3image == NET)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4image == NET)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }

        }
        if(im->bins[3].binSeen && binSecondary == TRIDENT)
        {
            binsSecondaryFound = true;
            if(im->target1image == TRIDENT)
            {
                x2 = im->x1;
                y2 = im->y1;
                z2 = im->z1;
                heading2 = im->heading1;
            }
            if(im->target2image == TRIDENT)
            {
                x2 = im->x2;
                y2 = im->y2;
                z2 = im->z2;
                heading2 = im->heading2;
            }
            if(im->target3image == TRIDENT)
            {
                x2 = im->x3;
                y2 = im->y3;
                z2 = im->z3;
                heading2 = im->heading3;
            }
            if(im->target4image == TRIDENT)
            {
                x2 = im->x4;
                y2 = im->y4;
                z2 = im->z4;
                heading2 = im->heading4;
            }
        }
    }

	return 1; // should it return this or something else?
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

