#include "../core/image_kb.h"
#include "../core/image_kb.cpp"
//#include "match.cpp"
#include "headers.cpp"
#include "rectangleUtils.cpp"
#include "gate.cpp"
#include "buoys.cpp"
#include "path.cpp"

void updateKB(IMAGE_KB *kb)
{
  Mat query;
  Buoy buoys[3];
  int numBuoys = 0;

#ifdef DEBUG
  printf("checking gate\n");
#endif

  // Hold coordinates of images seen in the following image matching functions
  float leftPostX = 0, rightPostX = 0, distance = 0;

  // Find the gate
  if(checkGate(&leftPostX, &rightPostX, &distance)){
    // left and right pillar are seen
    kb->sgPillars[0].pillarSeen = true; // left
    kb->sgPillars[1].pillarSeen = true; // right

    // set their X coordinates
    kb->sgPillars[0].pillarX = leftPostX;
    kb->sgPillars[1].pillarX = rightPostX;

    // set the distance
    kb->startGateCenterDistance = distance;

#ifdef DEBUG
    printf("leftX = %f\n", leftPostX);
    printf("rightX = %f\n", rightPostX);
    printf("distance = %f\n", distance);
#endif

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
    kb->numBuoysSeen = numBuoys;
  }

  double pathDegrees = 0;
  if(checkPath(&pathDegrees))
  {
    kb->rightPathHeading = pathDegrees;
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
