#ifndef __VISION__
#define __VISION__

//#include "../core/image_kb.h"
#include "checkImage.h"
#include "../core/core.h"

// only need this when testing
#ifdef TEST
//#include "../core/image_kb.cpp"
#endif

//#include "match.cpp"
#include "utilities/headers.cpp"
#include "utilities/rectangleUtils.cpp"
#include "obstacleFunctions/gate.cpp"
#include "obstacleFunctions/buoys.cpp"
#include "obstacleFunctions/path.cpp"

void v_startGate(IMAGE_KB *im)
{
#ifdef DEBUG
  printf("checking gate\n");
#endif

  // Hold coordinates of images seen in the following image matching functions
  // X coordinates are the pixel values
  float leftPostX = 0, rightPostX = 0, distance = 0;

  // Find the gate
  if(im->gateObstacle && checkGate(&leftPostX, &rightPostX, &distance))
  {
    // left and right pillar are seen
    im->sgPillars[0].pillarSeen = true; // left
    im->sgPillars[1].pillarSeen = true; // right

    // set their X coordinates
    im->sgPillars[0].pillarX = leftPostX;
    im->sgPillars[1].pillarX = rightPostX;

    // set the distance
    im->startGateCenterDistance = distance;

#ifdef DEBUG
    printf("leftX = %f\n", leftPostX);
    printf("rightX = %f\n", rightPostX);
    printf("distance = %f\n", distance);
#endif

  }
}

void v_path(IMAGE_KB *im)
{
#ifdef DEBUG
  printf("checking path\n");
#endif
  double pathDegrees = 0;

  if(im->pathObstacle && checkPath(&pathDegrees))
  {
    im->rightPathHeading = pathDegrees;
  }
}

void v_buoys(IMAGE_KB *im)
{
  // Find buoys
#ifdef DEBUG
  printf("checking buoys\n");
#endif

  Buoy buoys[3];
  int numBuoys = 0;

  if(im->buoyObstacle && checkBuoys(buoys, &numBuoys))
  {
    for(int i = 0; i < numBuoys; i++)
    {
      im->buoys[i].buoySeen = true;
      im->buoys[i].buoyX = buoys[i].buoyX;
      im->buoys[i].buoyY = buoys[i].buoyY;
      im->buoys[i].buoyZ = buoys[i].buoyZ;
      im->buoys[i].buoyColor = buoys[i].buoyColor;
    }
    im->numBuoysSeen = numBuoys;
  }
}


// only need a main function when testing, otherwise the AI just includes
// this file and calls the function
#ifdef TEST
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
#endif
#endif
