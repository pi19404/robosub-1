#include "../core/image_kb.h"
#include "../core/image_kb.cpp"
#include <iostream>

//#include "match.cpp"
#include "gate.cpp"
#include "buoys.cpp"

using namespace std;

void updateKB(IMAGE_KB *kb)
{
  Mat query;
  Buoy buoys[3];
  int numBuoys = 0;

#ifdef DEBUG
  printf("checking gate\n");
#endif

  // Hold coordinates of images seen in the following image matching functions
  float leftPostX = 0, rightPostX = 0;

  // Find the gate
  if(checkGate(&leftPostX, &rightPostX)){
    // left and right pillar are seen
    kb->sgPillars[0].pillarSeen = true; // left
    kb->sgPillars[1].pillarSeen = true; // right

    // set their X coordinates
    kb->sgPillars[0].pillarX = leftPostX;
    kb->sgPillars[1].pillarX = rightPostX;
  }

  // Find buoys
#ifdef DEBUG
  printf("checking buoys\n");
#endif

  if(checkBuoys(buoys, &numBuoys)){
    for(int i = 0; i < numBuoys; i++)
    {
      kb->buoys[i].buoySeen = true;
      kb->buoys[i].buoyX = buoys[i].buoyX;
      kb->buoys[i].buoyY = buoys[i].buoyY;
      kb->buoys[i].buoyZ = buoys[i].buoyZ;
      kb->buoys[i].buoyColor = buoys[i].buoyColor;
    }
  }
}

/******************************************************************** 
 * For testing.
 * 1. Hold an image in front of the camera.
 * 2. Run the program.
 * 3. Look at the changed state of the KB object to determine if
 *    the expected results occured.
********************************************************************/
int main()
{
  IMAGE_KB *knowledge = new IMAGE_KB();
  updateKB(knowledge);

  // compare results to expected results to determine correctness of test
  if(knowledge->sgPillars[0].pillarSeen && knowledge->sgPillars[1].pillarSeen)
    printf("Test 1 Passed!\n");

  if(knowledge->buoys[0].buoySeen && knowledge->buoys[1].buoySeen &&
     knowledge->buoys[0].buoyColor == RED && knowledge->buoys[1].buoyColor == YELLOW)
    printf("Test 2 Passed!\n");

  return 0;
}
