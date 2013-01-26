/* Knowledge Base cpp */

#include "kb.h"

KB::KB()
{
        // Multiple Tasks
        StartGateComplete = false;
        BuoyTaskComplete = false;
        ObstacleCourse1Complete = false;
        TorpedoTaskComplete = false;
        BinsTaskComplete = false;
        ObstacleCourse2Complete = false;

        TargetInRange = false; // Torpedos, Bins
        AttemptTask = false;

        // Start Gate  // Used for Obstacle Course Too
        Pillar1Found = false;
        Pillar2Found = false;

        // Paths

        // Buoys
        CorrectHeading = false;
        BuoyRedSeen = false;
        BuoyGreenFound = false;
        BuoyRedFound = false;
        BuoyYellowFound = false;
        PrimaryBuoyComplete = false;
        SecondaryBuoyComplete = false;
        
        // Obstacle Course
        BarFound = false;

        // re use gate pillar booleans

        // Torpedoes
        PrimaryTorpedoTargetComplete = false;
        SecondaryTorpedoTargetComplete = false;

        // Bins
        BinsFound  = false;
        BinsPrimaryFound = false;
        PrimaryBinTargetComplete = false;
        BinsSeondaryFound = false;
        SecondaryBinTargetComplete = false;
        DistancesCalculated = false;

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
        buoyPrimary   = DEFAULT;
        buoySecondary = DEFAULT;
        
        // Torpedo Targets
        torpedoPrimary   = DEFAULT;
        torpedoSecondary = DEFAULT;

        // Bin Targets
        binPrimary   = UNKNOWN;
        binSecondary = UNKNOWN;

        return this;
}

// update KB found based on image recognition
KB::int updateKB(IMAGE_KB *im)
{
    // update found
    if(im->Pillar1Seen)
        Pillar1Found = true;
    if(im->Pillar2Seen)
        Pillar2Found = true;

    // Set Primary Buoy Target
    if(im->BuoyGreenSeen && buoyPrimary == GREEN)
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
    if(im->BuoyRedSeen && buoyPrimary == RED)
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
    if(im->BuoyYellowSeen && buoyPrimary == YELLOW)
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
        if(im->BuoyGreenSeen && buoySecondary == GREEN)
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
        if(im->BuoyRedSeen && buoySecondary == RED)
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
        if(im->BuoyYellowSeen && buoySecondary == YELLOW)
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

    // Update Bins Found
    if(im->BinsSeen)
        BinsFound = true;

    if(im->swordSeen && binPrimary == SWORD)
    {
        BinsPrimaryFound = true;

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
    if(im->shieldSeen && binPrimary == SHIELD)
    {
        BinsPrimaryFound = true;
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
    if(im->netSeen && binPrimary == NET)
    {
        BinsPrimaryFound = true;
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
    if(im->tridentSeen && binPrimary == TRIDENT)
    {
        BinsPrimaryFound = true;
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


    if(kb->BinsPrimaryFound)
    {
        if(im->swordSeen && binSecondary == SWORD)
        {
            BinsSecondaryFound = true;

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
        if(im->shieldSeen && binSecondary == SHIELD)
        {
            BinsSecondaryFound = true;
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
        if(im->netSeen && binSecondary == NET)
        {
            BinsSecondaryFound = true;
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
        if(im->tridentSeen && binSecondary == TRIDENT)
        {
            BinsSecondaryFound = true;
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
}
