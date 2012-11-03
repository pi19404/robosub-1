/* Core AI file */

#include <stdio.h>
#include "kb.h"

int main(int argc, char* argv[])
{
    // Setup
    KB kb; // Set Knowledge Base
    
    // add error handling
    while(1)
        mainLoop(KB *kb);
}

int mainLoop(KB *kb)
{
    if (!kb->StartGateComplete)
    {
        StartGate(KB *kb);
    }
    else if (!kb->BuoysComplete)
    {
        Buoys(KB *kb);
    }
    else if (!kb->ObstacleCourse1Complete)
    {
        ObstacleCourse(KB *kb);
    }
    else if (!kb->TorpedoTaskComplete)
    {
        Torpedos(KB *kb);
    }
    else if (!kb->BinsTaskComplete)
    {
        Bins(KB *kb);
    }
    else if (!kb->ObstacleCourse2Complete)
    {
        ObstacleCourse(KB *kb);
    }
}


int StartGate(KB *kb)
{
    if( kb->Pillar1Seen && !kb->Pillar1Found )
    {
        kb->Pillar1Found = true;
    }
    if( kb->Pillar2Seen && !kb->Pillar2Found )
    {
        kb->Pillar2Found = true;
    }
    if( kb->Pillar1Found && kb->Pillar2Found && ( kb->Pillar1Seen || kb->Pillar2Seen ))
    {
        // don't need to rotate, so heading is 0
        // y is set to 1 to move forward
        // update x and z from image recognition

        move(kb->x,kb->y,kb->z,heading);
    }
    if( kb->Pillar1Found && kb->Pillar2Found && !kb->Pillar1Seen && !kb->Pillar2Seen )
    {
        kb->StartGateComplete = true;
    }
}

int Paths(KB *kb)
{

    // get all from image recognition

    if (numPaths > 1)
    {
        // follow the right one

    }
    // else just follow path
}

int Buoys(KB *kb)
{
    if( kb->AttemptTask)
    {
        if( !kb->PrimaryBuoyComplete)
        {
            // move towards primary
            // get heading, x, y, z from image recognition

            move(kb->x,kb->y,kb->z,heading);
            
        }
        else if( !kb->SecondaryBuoyComplete)
        {
            // move towards secondary

        }

        if (kb->PrimaryBuoyComplete && kb->SecondaryBuoyComplete)
        {
            kb->BuoyTaskComplete = true;
            kb->AttemptTask = false;
        }
    }
    if( !kb->BuoyGreenFound && kb->BuoyGreenSeen )
        kb->BuoyGreenFound = true;
    if( !kb->BuoyRedFound && kb->BuoyRedSeen )
        kb->BuoyRedFound = true;
    if( !kb->BuoyYellowFound && kb->BuoyYellowSeen )
        kb->BuoyYellowFound = true;
    
    if( kb->BuoyGreeFound || kb->BuoyRedFound || kb->BuoyYellowFound )
    {
        //compareSize(); // in case we got too close without seeing it
        // if one found if currentSize > goalSize -> RotateStrafe(90degrees, BuoyX)
         
        if( kb->BuoyGreeFound && kb->BuoyRedFound && kb->BuoyYellowFound )
        {
            kb->AttemptTask = true; 
        }
    }


}
