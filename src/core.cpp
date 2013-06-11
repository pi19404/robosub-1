/* Core AI file */

#include "core.h"

using namespace std;

#define FORWARD 1	// Move forward at a constant speed
#define REVERSE -1	// Move in reverse at a constant speed
#define UP 1		// Move up at a constant speed
#define DOWN -1		// Move down at a constant speed
#define DEPTH   5	// Default depth, 5 ft
#define HEADING_FORWARD 0 // Default forward heading

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
        aiMainLoop(kb, im);

    return 1;
}

int aiMainLoop(KB *kb, IMAGE_KB *im)
{
	im->initializeIM(im);	// reset the Image KB
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
    if(im->sgPillars[0].pillarSeen && !kb->pillar1Found)
    {
        kb->pillar1Found = true;
    }
    if(im->sgPillars[1].pillarSeen && !kb->pillar2Found)
    {
        kb->pillar2Found = true;
    }
    if(kb->pillar1Found && kb->pillar2Found && ( im->sgPillars[0].pillarSeen || im->sgPillars[1].pillarSeen))
    {
        // don't need to rotate, so heading is 0
        // y is set to 1 to move forward
        // update x and z from image recognition

        move(kb->x1,kb->y1,kb->z1,kb->heading1);
    }
    if(kb->pillar1Found && kb->pillar2Found && !im->sgPillars[0].pillarSeen && !im->sgPillars[1].pillarSeen)
    {
        kb->startGateComplete = true;
		// continue moving forward (to find the path) for a specified amount of time, maybe wait for a bit once we dont see the pillars anymore
		/*wait()*/
    }
    return 0;
}

int Paths(KB *kb, IMAGE_KB *im)
{
    // get all from image recognition

    /*if (im->twoPaths)
    {
        // follow the right one // Verify the right one is on the right
        if(im->leftPathX > im->rightPathX)
        {
            move(im->leftPathX, DEPTH, FORWARD, im->leftPathHeading);
        }
        else
        {
            move(im->rightPathX, DEPTH, FORWARD, im->rightPathHeading);
        }
    }*/
    // else just follow path
    if (im->pathSeen)
    {
        move(im->rightPathX, DEPTH, FORWARD, im->rightPathHeading);
    }
    return 0;
}

int Buoys(KB *kb, IMAGE_KB *im)
{ 
    // Look for all buoys (want to see all three at once)

    if ( !kb->buoy1Hit && !kb->buoy2Hit && !kb->buoy3Hit )   // buoy 1,2,3 haven't all been hit continue with buoy task
    {
        if ( im->buoys[0].buoySeen && im->buoys[1].buoySeen && im->buoys[2].buoySeen )
        {
            // Once all three are found: Pick the farthest left buoy not hit yet. 
            if ( !kb->buoy1Hit )
            {
                // Move towards that buoy
                move( im->buoys[0].buoyX, im->buoys[0].buoyY, im->buoys[0].buoyZ, HEADING_FORWARD );
                // Once in range (about 2 ft away)
                if ( im->buoys[0].buoyZ < 2 )
                {
                    // Wait ( ie stop moving )
                    move( 0, 0, 0, 0 ); // Might need to shorty reverse to stop forward momentum
                    // Check the color of the buoy, // learn the cycle? 
                    if ( im->buoys[0].buoyColor == kb->buoyGoalColor )
                    {
                        // As soon as the correct color appears, move forward to hit it
                        move( im->buoys[0].buoyX, im->buoys[0].buoyY, FORWARD, HEADING_FORWARD );
                        // TODO wait enough time for buoy to be hit
                        /*wait()*/
						kb->buoy1Hit = true;
                        // Once the buoy is hit, reverse
                        move( im->buoys[0].buoyX, im->buoys[0].buoyY, REVERSE, HEADING_FORWARD );
                    }
                }
            }
            else if ( !kb->buoy2Hit )
            {
                // Move towards that buoy
                move( im->buoys[1].buoyX, im->buoys[1].buoyY, im->buoys[1].buoyZ, HEADING_FORWARD );
                // Once in range (about 2 ft away)
                if ( im->buoys[1].buoyZ < 2 )
                {
                    // Wait ( ie stop moving )
                    move( 0, 0, 0, 0 ); // Might need to shorty reverse to stop forward momentum
                    // Check the color of the buoy, // learn the cycle? 
                    if ( im->buoys[1].buoyColor == kb->buoyGoalColor )
                    {
                        // As soon as the correct color appears, move forward to hit it
                        move( im->buoys[1].buoyX, im->buoys[1].buoyY, FORWARD, HEADING_FORWARD );
                        // TODO wait enough time for buoy to be hit
                        /*wait()*/
						kb->buoy2Hit = true;
                        // Once the buoy is hit, reverse
                        move( im->buoys[1].buoyX, im->buoys[1].buoyY, REVERSE, HEADING_FORWARD );
                    }
                }
            }
            else if ( !kb->buoy3Hit )
            {
                // Move towards that buoy
                move( im->buoys[2].buoyX, im->buoys[2].buoyY, im->buoys[2].buoyZ, HEADING_FORWARD );
                // Once in range (about 2 ft away)
                if ( im->buoys[2].buoyZ < 2 )
                {
                    // Wait ( ie stop moving )
                    move( 0, 0, 0, 0 ); // Might need to shorty reverse to stop forward momentum
                    // Check the color of the buoy, // learn the cycle?
                    if ( im->buoys[2].buoyColor == kb->buoyGoalColor )
                    {
                        // As soon as the correct color appears, move forward to hit it
                        move( im->buoys[2].buoyX, im->buoys[2].buoyY, FORWARD, HEADING_FORWARD );
                        // TODO wait enough time for buoy to be hit
                        /*wait()*/
						kb->buoy3Hit = true;
                        // Once the buoy is hit, reverse
                        move( im->buoys[2].buoyX, im->buoys[2].buoyY, REVERSE, HEADING_FORWARD );
                    }
                }
            }
        }
		else // all 3 not seen
		{
			// TODO add error handling for if, for example
			// , we dont see all three: we see only 1 or we never see any
            // continue moving till we see all 3
		}
    }
    else // All three buoys hit
    {
		kb->buoyTaskComplete = true;
        // Move back TODO add the timings in here
        move( 0, 0, REVERSE, HEADING_FORWARD );
		/*wait()*/
        // Move up above the buoys
        move( 0, UP, 0, HEADING_FORWARD );
		/*wait()*/
        // Move forward over the buoys
        move( 0, 0, FORWARD, HEADING_FORWARD );
		/*wait()*/
    }

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

bool move(int x, int y, int z, double heading)
{
	bool result = false;
    cout << "X: " << x << " Y: " << y << " Z: " << z << " Heading: " << heading << endl;
    // TODO Send to controller code

	return result;
}
