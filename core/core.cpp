/* Core AI file */

#include "core.h"

int main(int argc, char* argv[])
{
    // Setup
    KB kb; // Set Knowledge Base
    
    // add error handling
    while(1)
        mainLoop(kb, im);

    return 1;
}

int mainLoop(KB *kb, IMAGE_KB *im)
{
    kb->updateKB();

    if (!kb->StartGateComplete)
    {
        StartGate(kb, im);
    }
    else if (!kb->BuoysComplete)
    {
        Paths(kb);
        Buoys(kb);
    }
    else if (!kb->ObstacleCourse1Complete)
    {
        Paths(kb);
        ObstacleCourse(kb);
    }
    else if (!kb->TorpedoTaskComplete)
    {
        Paths(kb);
        Torpedos(kb);
    }
    else if (!kb->BinsTaskComplete)
    {
        Paths(kb);
        Bins(kb);
    }
    else if (!kb->ObstacleCourse2Complete)
    {
        Paths(kb);
        ObstacleCourse(kb);
    }
    return 0;
}


int StartGate(KB *kb, IMAGE_KB *im)
{
    if(im->Pillar1Seen && !kb->Pillar1Found)
    {
        kb->Pillar1Found = true;
    }
    if(im->Pillar2Seen && !kb->Pillar2Found)
    {
        kb->Pillar2Found = true;
    }
    if(kb->Pillar1Found && kb->Pillar2Found && ( im->Pillar1Seen || im->Pillar2Seen))
    {
        // don't need to rotate, so heading is 0
        // y is set to 1 to move forward
        // update x and z from image recognition

        move(kb->x1,kb->y1,kb->z1,heading1);
    }
    if(kb->Pillar1Found && kb->Pillar2Found && !im->Pillar1Seen && !im->Pillar2Seen)
    {
        kb->StartGateComplete = true;
    }
    return 0;
}

int Paths(KB *kb)
{
    // get all from image recognition

    if (kb->twoPaths)
    {
        // follow the right one
        if(kb->x1 > kb->x2)
        {
            move(kb->x1, kb->y1, kb->z1, kb->heading1);
        }
        else
        {
            move(kb->x2, kb->y2, kb->z2, kb->heading2);
        }
    }
    // else just follow path
    move(kb->x1, kb->y1, kb->z1, kb->heading1);
    return 0;
}

int Buoys(KB *kb, IMAGE_KB *image_kb)
{
    if(kb->AttemptTask) // Buoys found and labeled, do the task now
    {
        if(!kb->PrimaryBuoyComplete)
        {
            // move towards primary
            // get heading, x, y, z from image recognition

            //  if primary is found
            if(kb->buoyPrimaryFound)
            {
                move(kb->x1,kb->y1,kb->z1,heading1);
                if(!im->BuoyGreenSeen && !im->BuoyRedSeen && !im->BuoyYellowSeen)
                {
                    // TODO wait x time
                    // to guarantee running into the buoy
                    // Run tests to determine x
                    kb->PrimaryBuoyComplete = true;
                    // Move backwards
                    move(0,-1,0, 0);
                    // Wait y amount of time
                }
            }
            else
            {
                // continue following heading
            }
        }
        else if(!kb->SecondaryBuoyComplete)
        {
            // move towards secondary
            if(kb->buoySecondaryFound)
            {
                move(kb->x2,kb->y2,kb->z2,heading2);
                if(!im->BuoyGreenSeen && !im->BuoyRedSeen && !im->BuoyYellowSeen)
                {
                    // TODO wait x time
                    // to guarantee running into the buoy
                    // Run tests to determine x
                    kb->SecondaryBuoyComplete = true;
                    while(depth < x)
                    {
                        // Move backwards and up
                        move(0,-1,-1, 0);
                    }
                    //Move Forward to look for path
                    move(0,1,0,0);
            }
            else
            {
                // continue following heading
            }

        }

        if (kb->PrimaryBuoyComplete && kb->SecondaryBuoyComplete)
        {
            kb->BuoyTaskComplete = true;
            kb->AttemptTask = false;
        }
    }
    
   /* if(kb->BuoyGreeFound || kb->BuoyRedFound || kb->BuoyYellowFound)
    {
        //compareSize(); // in case we got too close without seeing it
        // if one found if currentSize > goalSize -> RotateStrafe(90degrees, BuoyX)
         
        if(kb->BuoyGreeFound && kb->BuoyRedFound && kb->BuoyYellowFound)
        {
            kb->AttemptTask = true; 
        }
    }
    */

    }
    return 0;
}

int ObstacleCourse(KB *kb)
{

    return 0;
}


int Torpedoes(KB *kb)
{

    return 0;
}

int Bins(KB *kb, IMAGE_KB *im)
{

    return 0;
}
