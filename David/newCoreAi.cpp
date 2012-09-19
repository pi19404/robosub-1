#include "newCoreAI.h"
#include "sub.h"

// GLOBALS
#define CEILING 158
#define FLOOR   156

Sub *sub;

/* Would likely be in header if used
typedef struct Obst
{  // Obstacle struct to hold 
   string name;
   uint8_t  status; //status
   uint16_t depth; //0-1023

   uint8_t  heading; //offset heading
   
}*/

int main (void)
{
    sub = new Sub("/dev/ttyACM0", "9600");

    wait_for_armed();

    // -127 to 127 values
    sub->PowerY = 70;   // Move forward
    sub->update(); 

    while (true)
    {
        mainLoop();
    }

    return 0;
}

// Functions
void checkDepth() // Set for gate obstacle
    if (sub->Depth < CEILING)
    {
        sub->PowerZ = -70; // Setting power values for thrusters
    }
    else if (sub->Depth > FLOOR)
    {
        sub->PowerZ = 70; // Setting power values for thrusters
    }
    else
    {
        sub->PowerZ = 0; // Setting power values for thrusters
    }
}

void wait_for_armed() // Wait for the start button to be pressed
{
    while (sub.status == false)
    {
       usleep(250000);
       sub->update();
    }
}

void mainLoop()
{
    checkDepth();
    sub->update();

    findGates();
    sub->update();

}
