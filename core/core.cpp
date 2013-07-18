/* Core AI file */

#include "core.h"
#include "../AIFunctionCall/checkImage.cpp"
#include "../control_sys/robosub_controller.h"

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

#include <unistd.h>

#include <boost/program_options.hpp>
#include <boost/bind.hpp>

using namespace std;

#define FORWARD 1         // Move forward at a constant speed
#define FORWARD_FAST 2    // Move forward at full speed
#define REVERSE 3         // Move in reverse at a constant speed
#define REVERSE_FAST 4    // Move backwards at full speed
#define UP 1              // Move up at a constant speed
#define UP_FAST 2         // Move up at full speed
#define DOWN 3            // Move down at a constant speed
#define DOWN_FAST 4       // Move down at full speed
#define DEPTH 5           // Default depth, 5 ft
#define HEADING_FORWARD 0 // Default forward heading
#define STOP 0            // STOP!! :D

void recv_callback( const ArduinoData& data )
{
    cout << "Status Received from Arduino: " << endl;
    cout << data << endl;
}

void send_callback( const RoboSubCommand& data )
{
    cout << "Command Sent to Aruino: " << endl;
    cout << data << endl;
}

// Initialize the control communication
RoboSubController controller;
controller.AttachDataReceivedCallback(recv_callback);
controller.AttachDataSentCallback(send_callback);

int main(int argc, char* argv[])
{
    // Setup
    KB *kb; // = new KB();//(sizeof(KB)); // Set Knowledge Base
    IMAGE_KB *im; // = new IMAGE_KB();// (sizeof(IMAGE_KB)); // Set Image KB
    
    kb = new KB();
    im = new IMAGE_KB();
    
	im->initializeIM(im); // initialize the image KB

    // add error handling
    while(1)
        aiMainLoop(kb, im);

    return 1;
}

int aiMainLoop(KB *kb, IMAGE_KB *im)
{

    kb->updateKB(im);     // Update the
	im->initializeIM(im); // reset the Image KB
    
    if (!kb->startComplete)
    {
        // Very beginning the sub should move forward and down to depth
        move(0, FORWARD, DEPTH, 0, false, false, false, false, false);

        kb->startComplete = true;
    }
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
    if(kb->pillar1Found && kb->pillar2Found && ( im->sgPillars[0].pillarSeen || im->sgPillars[1].pillarSeen))
    {
        // don't need to rotate, so heading is 0
        // y is set to 1 to move forward
        // update x and z from image recognition

        move(kb->x1,kb->y1,kb->z1,kb->heading1, false, false, false, false, false);
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

    //follow path
    if (im->pathSeen)
    {
        move(im->rightPathX, DEPTH, FORWARD, im->rightPathHeading, false, false, false, false, false);
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
                move( im->buoys[0].buoyX, im->buoys[0].buoyY, im->buoys[0].buoyZ, HEADING_FORWARD, false, false, false, false, false );
                // Once in range (about 2 ft away)
                if ( im->buoys[0].buoyZ < 2 )
                {
                    // Wait ( ie stop moving )
                    move( 0, 0, 0, 0, false, false, false, false, false ); // Might need to shorty reverse to stop forward momentum
                    // Check the color of the buoy, // learn the cycle? 
                    if ( im->buoys[0].buoyColor == kb->buoyGoalColor )
                    {
                        // As soon as the correct color appears, move forward to hit it
                        move( im->buoys[0].buoyX, im->buoys[0].buoyY, FORWARD, HEADING_FORWARD, false, false, false, false, false );
                        // TODO wait enough time for buoy to be hit
                        /*wait()*/
						kb->buoy1Hit = true;
                        // Once the buoy is hit, reverse
                        move( im->buoys[0].buoyX, im->buoys[0].buoyY, REVERSE, HEADING_FORWARD, false, false, false, false, false );
                    }
                }
            }
            else if ( !kb->buoy2Hit )
            {
                // Move towards that buoy
                move( im->buoys[1].buoyX, im->buoys[1].buoyY, im->buoys[1].buoyZ, HEADING_FORWARD, false, false, false, false, false );
                // Once in range (about 2 ft away)
                if ( im->buoys[1].buoyZ < 2 )
                {
                    // Wait ( ie stop moving )
                    move( 0, 0, 0, 0, false, false, false, false, false); // Might need to shorty reverse to stop forward momentum
                    // Check the color of the buoy, // learn the cycle? 
                    if ( im->buoys[1].buoyColor == kb->buoyGoalColor )
                    {
                        // As soon as the correct color appears, move forward to hit it
                        move( im->buoys[1].buoyX, im->buoys[1].buoyY, FORWARD, HEADING_FORWARD, false, false, false, false, false );
                        // TODO wait enough time for buoy to be hit
                        /*wait()*/
						kb->buoy2Hit = true;
                        // Once the buoy is hit, reverse
                        move( im->buoys[1].buoyX, im->buoys[1].buoyY, REVERSE, HEADING_FORWARD, false, false, false, false, false );
                    }
                }
            }
            else if ( !kb->buoy3Hit )
            {
                // Move towards that buoy
                move( im->buoys[2].buoyX, im->buoys[2].buoyY, im->buoys[2].buoyZ, HEADING_FORWARD, false, false, false, false, false );
                // Once in range (about 2 ft away)
                if ( im->buoys[2].buoyZ < 2 )
                {
                    // Wait ( ie stop moving )
                    move( 0, 0, 0, 0, false, false, false, false, false ); // Might need to shorty reverse to stop forward momentum
                    // Check the color of the buoy, // learn the cycle?
                    if ( im->buoys[2].buoyColor == kb->buoyGoalColor )
                    {
                        // As soon as the correct color appears, move forward to hit it
                        move( im->buoys[2].buoyX, im->buoys[2].buoyY, FORWARD, HEADING_FORWARD, false, false, false, false, false );
                        // TODO wait enough time for buoy to be hit
                        /*wait()*/
						kb->buoy3Hit = true;
                        // Once the buoy is hit, reverse
                        move( im->buoys[2].buoyX, im->buoys[2].buoyY, REVERSE, HEADING_FORWARD, false, false, false, false, false );
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
        move( 0, 0, REVERSE, HEADING_FORWARD, false, false, false, false, false );
		/*wait()*/
        // Move up above the buoys
        move( 0, UP, 0, HEADING_FORWARD, false, false, false, false, false );
		/*wait()*/
        // Move forward over the buoys
        move( 0, 0, FORWARD, HEADING_FORWARD, false, false, false, false, false );
		/*wait()*/
    }

    return 0;
}

int ObstacleCourse(KB *kb)
{
    // Find bars

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
                move(0,0,0,0, false, false, false, false, false);
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


int Torpedos(KB *kb)
{
    // Find targets
    // TODO
    // Find primary target
        // Move to position in front of small hexagon
        // Aim
        // First torpedo 1
    // Find secondary target
        // Move to position in front of small hexagon
        // Aim
        // First torpedo 2
    
    // Move towards paths or bins if not already complete

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
    return 0;
}

bool move( int x, int y, int depth, double heading, 
           bool t1, bool t2, bool m1, bool m2, bool claw )
{
    // Interpret inputs
    static const unsigned int X = ThrustMode::STOP;
    static const unsigned int Y = ThrustMode::STOP;
    static const unsigned int torpedo1 = PneumMode::DEACTIVATED;
    static const unsigned int torpedo2 = PneumMode::DEACTIVATED;
    static const unsigned int marker1  = PneumMode::DEACTIVATED;
    static const unsigned int marker2  = PneumMode::DEACTIVATED;
    static const unsigned int claw     = PneumMode::DEACTIVATED;

    if ( x < 0 )
    {
        X = ThrustMode::NEG_NORMAL;
    }
    else if ( x > 0 )
    {
        X = ThrustMode::POS_NORMAL;
    }
    if ( y < 0 )
    {
        Y = ThrustMode::NEG_NORMAL;
    }
    else if ( y > 0 )
    {
        Y = ThrustMode::POS_NORMAL;
    }
    if (t1)
    {
        torpedo1 = PneumMode::ACTIVATED;
    }
    if (t2)
    {
        torpedo2 = PneumMode::ACTIVATED;
    }
    if (m1)
    {
        marker1 = PneumMode::ACTIVATED;
    }
    if (m2)
    {
        marker2 = PneumMode::ACTIVATED;
    }
    if (c)
    {
        claw = PneumMode::ACTIVATED;
    }

	bool result = false;
   cout << "X: " << x << " Y: " << y << " Z: " << z << " Heading: " << heading << endl;

    string serial_dev;
    string baud_rate;
    string infile_name;

   /*
    * Parse the command line
    */
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
       ("help", "Show this help message.")
       ("serial_port,s", po::value<std::string>(&serial_dev)->required()
                       , "arduino serial port.")
       ("baud_rate,b", po::value<std::string>(&baud_rate)->required()
                     , "serial port baud rate.");


    po::positional_options_description posOptions;
    posOptions.add("infile", 1);

    po::variables_map vm;

    try 
    {
        po::store(po::command_line_parser(argc, argv).options(desc)
                        .positional(posOptions).run(), vm);

        if( vm.count("help") )
        {
            cout << desc << endl;
            return 0;
        }

        po::notify(vm);
    }
    catch(exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cout << desc << endl;
        return 1;
    }

   /*
    * Run the Controller. 
    */

    Controller.Run(serial_dev, baud_rate);
    
    Controller.SetThrustAll( x, //ThrustMode::STOP,     // X left right
                             y, //ThrustMode::POS_FULL, // Y forward back
                             depth, //ThrustMode::NEG_FULL, // Depth
                             heading ); //ThrustMode::STOP );   // Heading

    Controller.SetPneumAll( t1, //PneumMode::ACTIVATED,  // Torpedo1
                            t2, //PneumMode::DEFAULT,    // Torpedo2
                            m1, //PneumMode::DEFAULT,    // Marker1
                            m2, //PneumMode::ACTIVATED,  // Marker2
                            claw ); //PneumMode::ACTIVATED );// Claw
    
    Controller.SendCommand();

   // X left and right
   // Y forward back
   // Z up and down
   // Heading positive clockwise and negative counter clockwise
   // TODO Send to controller code

   //-128 to 128   // 0.5 
   // -100 to 100 for thrusters

	return result;
}
