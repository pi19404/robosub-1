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
  VideoCapture cap(0); // open the downward facing camera
  Buoys buoys[3];
  int numBuoys = 0;

  // make sure the cap opened
  if(!cap.isOpened())
  {
      cout << "failed to open camera" << endl;
      return;
  }

  // Hold coordinates of images seen in the following image matching functions
  float leftPostX = 0, rightPostX = 0;

  // Find the gate
  if(checkGate(cap, &leftPostX, &rightPostX)){
    // left and right pillar are seen
    kb->sgPillar[0].pillarSeen = true; // left
    kb->sgPillar[1].pillarSeen = true; // right

    // set their X coordinates
    kb->sgPillar[0].pillarX = leftPostX;
    kb->sgPillar[1].pillarX = rightPostX;
  }

  // Find buoys
  if(checkBuoys(cap, buoys, &numBuoys)){
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


  return 0;
}
