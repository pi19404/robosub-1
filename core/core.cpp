/* Core AI file */

#include "core.h"
#include "../AIFunctionCall/checkImage.cpp"

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
    /*if(im->sgPillars[0].pillarSeen && !kb->pillar1Found)
    {
        kb->pillar1Found = true;
    }
    if(im->sgPillars[1].pillarSeen && !kb->pillar2Found)
    {
        kb->pillar2Found = true;
    }*/
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

    int i = 0;
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
    /*if (im->pathSeen)
    {
        move(im->rightPathX, DEPTH, FORWARD, im->rightPathHeading);
    }*/

    //Find first false in pathFound. 
    for(i = 0; (kb->pathFound[i] == true) && (i < 8); i++);

    //Back up counter to the last true in the array. Move on this last true.
    if(i > 0)
    {
	i -= 1;	
        move(im->paths[i].rightPathX, DEPTH, FORWARD, im->paths[i].rightPathHeading);
    }

    return 0;
}

int Buoys(KB *kb, IMAGE_KB *im)
{ 
    int i = 0, buoyHitCount = 0;

    if (!kb->buoyTaskComplete)   // buoy 1,2,3 haven't all been hit continue with buoy task
    {

	for(i = 0; i < 3; i++)
	{
		if(im->buoys[i].buoySeen && !kb->buoyHit[i] && im->buoys[i].isCylinder)
		{
			//move towards first buoy	
			move(im->buoys[i].buoyX, im->buoys[i].buoyY, im->buoys[i].buoyZ, HEADING_FORWARD);
			
			//stop when buoy is in range (about 2 ft away)
			if(im->buoys[i].buoyZ < 2)
			{	
				//stop moving / wait
				move(0, 0, 0, 0);
				
				if(im->buoys[i].buoyColor == kb->buoyGoalColor)
				{
					//Move towards buoy at full force if color is correct.
					move(im->buoys[i].buoyX, im->buoys[i].buoyY, FORWARD, HEADING_FORWARD);
					//record hit information, then move backwards			
					kb->buoyHit[i] = true;
					buoyHitCount++;
					move(im->buoys[i].buoyX, im->buoys[i].buoyY, REVERSE, HEADING_FORWARD);
				}
			}
		}
		else if(im->buoys[i].buoySeen && kb->buoyHit[i] && im->buoys[i].isCylinder)
		{
			buoyHitCount++;
		}
	

	if(buoyHitCount == 2)
	{
		kb->buoyTaskComplete = true;
		
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

int ObstacleCourse(KB *kb, IMAGE_KB *im)
{
    // Find bars
    if(im->horizBarSeen && !kb->horizBarFound)
    {
        kb->horizBarFound = true;  
    }
    if(im->leftBarSeen && !kb->leftBarFound)
    {
        kb->leftBarFound = true;
    }
    if(im->rightBarSeen && !kb->rightBarFound)
    {
        kb->rightBarFound = true;
    }

    if(kb->horizBarFound && kb->leftBarFound && kb->rightBarFound &&
        ( im->horizBarSeen || im->leftBarSeen || im->rightBarSeen ) )
    {
        // Move to the center of the obstacle lower half
        // if the bar is green 
        // if distance < 2ft and bar is green: move through
        // if distance < 2ft and bar is red: stop
        // if distance > 2ft move forward
        // TODO
        if ( im->horizBarZ < 2 )
        {
            if ( im->leftBarColor == GREEN && im->rightBarColor == GREEN)
            {
                // Move through the gate, lower half
                // TODO add parallel parking logic if straffing works well
                //TODO move();
            }
            else
            {
                // Move to the gate but not through it yet
                move(0,0,0,0);
            }
        }
        else 
        {
            // Move towards the center of the gate lower half 
            //TODO
            //move();

        }
    }
    if(kb->horizBarFound && kb->leftBarFound && kb->rightBarFound &&
        (!im->horizBarSeen && !im->leftBarSeen && !im->rightBarSeen ) )
    {
        kb->obstacleCourse1Complete = true;
		// continue moving forward (to find the path) for a specified amount of time, maybe wait for a bit once we dont see the pillars anymore
        // TODO
		/*wait()*/
    }

    return 0;
}


int Torpedos(KB *kb, IMAGE_KB *im)
{
    // TODO
    // Find targets
    // Find primary target
        // Move to position in front of small hexagon
        // Aim
        // First torpedo 1
    // Find secondary target
        // Move to position in front of small hexagon
        // Aim
        // First torpedo 2
    
    // Move towards paths or bins if not already complete
	int i = 0;

	if(!kb->primaryTorpedoTargetCompleted)
	{
		if(kb->primaryTorpedoTargetFound)
		{
			//Find location of primary
			for(i = 0; (im->torpedoTargets[i].isPrimary == false) && (i < 4); i++);

			//extra error checking
			if((i < 4) && (im->torpedoTargets[i].targetSeen == true))
			{
				//Move toward target
				move(im->torpedoTargets[i].targetX, im->torpedoTargets[i].targetY, im->torpedoTargets[i].targetZ, HEADING_FORWARD);
				
				if(im->torpedoTargets[i].targetZ < 2)
				{
					//FIREEEEEEE

					//Move backwards out of the way
					move(im->torpedoTargets[i].targetX, im->torpedoTargets[i].targetY, REVERSE, HEADING_FORWARD);
					kb->primaryTorpedoTargetCompleted = true;
				}
			}
		}	
	}

	if(!kb->secondaryTorpedoTargetCompleted)
	{
		if(kb->secondaryTorpedoTargetFound)
		{
			for(i = 0; (im->torpedoTargets[i].isSecondary == false) && (i < 4); i++);

			if((i < 4) && (im->torpedoTargets[i].targetSeen == true))
			{
				move(im->torpedoTargets[i].targetX, im->torpedoTargets[i].targetY, im->torpedoTargets[i].targetZ, HEADING_FORWARD);

				if(im->torpedoTargets[i].targetZ < 2)
				{
					//FIREEEEE
					
					//move backwards out of the way 
					move(im->torpedoTargets[i].targetX, im->torpedoTargets[i].targetY, REVERSE, HEADING_FORWARD);
					kb->secondaryTorpedoTargetCompleted = true;
				}
				
			}
		}
	}

    return 0;
}

int Bins(KB *kb, IMAGE_KB *im)
{
    // TODO
    // Find Bins
    // Find primary target
        // Move directly above primary target 
        // Aim
        // Drop marker 1
    // Find secondary target
        // Move directly above secondary target 
        // Aim
        // Drop marker 2
	int i = 0;

	if(!kb->primaryBinTargetCompleted)
	{
		if(kb->binsPrimaryFound)
		{
			//Find location of primary
			for(i = 0; (im->bins[i].isPrimary == false) && (i < 4); i++);

			//extra error checking
			if((i < 4) && (im->bins[i].binSeen == true))
			{
				//Move toward target
				move(im->bins[i].binX, im->bins[i].binY, im->bins[i].binZ, HEADING_FORWARD);
				
				if(im->bins[i].binZ < 2)
				{
					//DROP!!

					//Move backwards out of the way
					move(im->bins[i].binX, im->bins[i].binY, REVERSE, HEADING_FORWARD);
					kb->primaryBinTargetCompleted = true;
				}
			}
		}	
	}

	if(!kb->secondaryBinTargetCompleted)
	{
		if(kb->binsSecondaryFound)
		{
			for(i = 0; (im->bins[i].isSecondary == false) && (i < 4); i++);

			if((i < 4) && (im->bins[i].binSeen == true))
			{
				move(im->bins[i].binX, im->bins[i].binY, im->bins[i].binZ, HEADING_FORWARD);

				if(im->bins[i].binZ < 2)
				{
					//DROP!!
					
					//move backwards out of the way 
					move(im->bins[i].binX, im->bins[i].binY, REVERSE, HEADING_FORWARD);
					kb->secondaryBinTargetCompleted = true;
				}
				
			}
		}
	}

    return 0;
}

bool move(int x, int y, int z, double heading)
{
	bool result = false;
   cout << "X: " << x << " Y: " << y << " Z: " << z << " Heading: " << heading << endl;
   // X left and right
   // Y forward back
   // Z up and down
   // Heading positive clockwise and negative counter clockwise
   // TODO Send to controller code

   //-128 to 128   // 0.5 
   // -100 to 100 for thrusters

	return result;
}
