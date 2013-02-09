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
		path1found = true;
        path2found = true;
        path3found = true;
        path4found = true;
        path5found = true;
        path6found = true;
        path7found = true;

        // Buoys
		correctHeading = false;
        buoy1Found = false;
        buoy2Found = false;
        buoy3Found = false;
        buoy1Complete = false;
        buoy2Complete = false;
        buoy3Complete = false;
        buoy1Color = colors::DEFAULT;
        buoy2Color = colors::DEFAULT; 
        buoy3Color = colors::DEFAULT;
        
        // Parking Obstacle
        horizBarFound = true;
        leftBarFound = true;
        rightBarFound = true;

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
		buoyPrimary = colors::DEFAULT;
        
        // Torpedo Targets
        torpedoPrimary   = colors::DEFAULT;
        torpedoSecondary = colors::DEFAULT;

        // Bin Targets
        binPrimary   = images::UNKNOWN;
        binSecondary = images::UNKNOWN;

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
