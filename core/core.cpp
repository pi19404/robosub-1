/* Core AI file */

#include "core.h"
#include "kb.h"
#include "image_kb.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Setup
    KB *kb;// = new KB();//(sizeof(KB)); // Set Knowledge Base
    IMAGE_KB *im;// = new IMAGE_KB();// (sizeof(IMAGE_KB)); // Set Image KB
    
    kb = new KB();
    im = new IMAGE_KB();
    //KB kb = kb1;
    //IMAGE_KB im = im1;
    //kb = new KB::KB();
    
    // add error handling
    while(1)
        mainLoop(kb, im);

    return 1;
}

int mainLoop(KB *kb, IMAGE_KB *im)
{
    kb->updateKB(im);

    if (!kb->startGateComplete)
    {
        StartGate(kb, im);
    }
    else if (!kb->buoyTaskComplete)
    {
        Paths(kb, im);
        Buoys(kb, im);
    }
    else if (!kb->obstacleCourse1Complete)
    {
        Paths(kb, im);
        ObstacleCourse(kb);
    }
    else if (!kb->torpedoTaskComplete)
    {
        Paths(kb, im);
        Torpedos(kb);
    }
    else if (!kb->binsTaskComplete)
    {
        Paths(kb, im);
        Bins(kb, im);
    }
    else if (!kb->obstacleCourse2Complete)
    {
        Paths(kb, im);
        ObstacleCourse(kb);
    }
    return 0;
}


int StartGate(KB *kb, IMAGE_KB *im)
{
    if(im->pillar1Seen && !kb->pillar1Found)
    {
        kb->pillar1Found = true;
    }
    if(im->pillar2Seen && !kb->pillar2Found)
    {
        kb->pillar2Found = true;
    }
    if(kb->pillar1Found && kb->pillar2Found && ( im->pillar1Seen || im->pillar2Seen))
    {
        // don't need to rotate, so heading is 0
        // y is set to 1 to move forward
        // update x and z from image recognition

        move(kb->x1,kb->y1,kb->z1,kb->heading1);
    }
    if(kb->pillar1Found && kb->pillar2Found && !im->pillar1Seen && !im->pillar2Seen)
    {
        kb->startGateComplete = true;
    }
    return 0;
}

int Paths(KB *kb, IMAGE_KB *im)
{
    // get all from image recognition

    if (im->twoPaths)
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

int Buoys(KB *kb, IMAGE_KB *im)
{
    if(kb->attemptTask) // Buoys found and labeled, do the task now
    {
        if(!(kb->buoy1Complete))
        {
            // move towards first buoy (left to right)
            // get heading, x, y, z from image recognition
            int buoyHeading = 0; // Should be zero, TODO verify with tests

            //  if primary is found
            if(im->buoy1Seen)
            {
                // Move close to buoy
                if (im->buoy1Z > 2 ) // if farther than 2 ft, move closer
                {
                    move(im->buoy1X, im->buoy1Y, im->buoy1Z, buoyHeading);
                }
                else
                {
                    // Pause until goal color // TODO update with tests 
                    // might need to stop forward momentum with a burst of reverse
                    move(0,0,0,0);
                    

                    // Detect color of buoy, pre hit, go for goal color
                    if(buoy1Color == buoyPrimary)
                    { 
                        // once changed to goal color
                            // immediately move to hit 
                        move(0,0,1,0); // move forward

                        // TODO Set up some sort of timer (preferably non halting)
                        /* Possible option to look into: http://stackoverflow.com/questions/5773088/making-a-timer-in-c
                        #include <chrono>
                        #include <iostream>

                        int main()
                        {
                            std::cout << "begin\n";
                            std::chrono::steady_clock::time_point tend = std::chrono::steady_clock::now()
                                                                       + std::chrono::minutes(1);
                            while (std::chrono::steady_clock::now() < tend)
                            {
                                // do your game
                            }
                            std::cout << "end\n";
                        }
                        */
                    }
                    // TODO set buoy1Complete = True; 
                    // TODO Move back after hitting the buoy
                       // Move to see both other buoys

                }
                // Record color hit

                // TODO determine hit buoy // size or percentage of buoy on screen

                // Once hit, reverse till all three are seen again


 
                // TODO fix this logic, doesn't look right
                if(!im->buoy1Seen && !im->buoy2Seen && !im->buoy3Seen)
                {
                    // TODO wait x time
                    // to guarantee running into the buoy
                    // Run tests to determine x
                    kb->primaryBuoyComplete = true;
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
        else if(!(kb->buoy2Complete))
        {
            // Repeat with buoy 2 
                // Hit when it's goal color
            // move towards secondary
            if(kb->buoy2Found)
            {
                move(kb->x2,kb->y2,kb->z2,im->heading2);
                if(!im->buoyGreenSeen && !im->buoyRedSeen && !im->buoyYellowSeen)
                {
                    // TODO wait x time
                    // to guarantee running into the buoy
                    // Run tests to determine x
                    kb->secondaryBuoyComplete = true;
                    while(kb->depth < kb->minDepth)
                    {
                        // Move backwards and up
                        move(0,-1,-1, 0);
                    }
                    //Move Forward to look for path
                    move(0,1,0,0);
                }
            }
            else
            {
                // continue following heading
            }

        }
        else if(!(kb->buoy3Complete))
        {
        // Repeat with buoy 3
            // Hit when it's goal color

        }

        if (kb->buoy1Complete && kb->buoy2Complete && kb->buoy3Complete)
        {
            kb->buoyTaskComplete = true;
            kb->attemptTask = false;
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

    //}
    return 0;
}

int ObstacleCourse(KB *kb)
{

    return 0;
}


int Torpedos(KB *kb)
{

    return 0;
}

int Bins(KB *kb, IMAGE_KB *im)
{

    return 0;
}

bool move(int x, int y, int z, int heading)
{

}
