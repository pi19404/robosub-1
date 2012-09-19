#include <stdio.h>
#include <unistd.h>
#include "sub.h"

// GLOBALS
#define CEILING 160
#define FLOOR 170

Sub *sub;

// Function Definitions
void checkDepth();
void wait_for_armed();
void mainLoop();

int main (void)
{
    sub = new Sub("/dev/ttyACM0", "9600");

    //wait_for_armed();

    //sub->y = 70;

    while (true)
    {
        mainLoop();
    }

    return 0;
}

// Functions 
void checkDepth() // Set for gate obstacle
{
    printf("Depth: %d\n", sub->rDepth);
    if (sub->rDepth < CEILING)
    {
        sub->z = -70;
    }
    else if (sub->rDepth > FLOOR)
    {
        sub->z = 70;
    }
    else
    {
        sub->z = 0;
    }
}

void wait_for_armed()
{
    while (sub->rStatus == false)
    {
       usleep(50000);
       sub->update();
    }
}

void mainLoop()
{
    checkDepth();
    sub->update();
    findGate();
    sub->update();
}
