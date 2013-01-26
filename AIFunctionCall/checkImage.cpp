#include "../core/image_kb.h"
#include "../core/image_kb.cpp"
#include <iostream>

//#include "match.cpp"
#include "gate.cpp"

using namespace std;

void updateKB(IMAGE_KB *kb)
{
  Mat query;
  VideoCapture cap(0); // open the downward facing camera

  // make sure the cap opened
  if(!cap.isOpened())
  {
      cout << "failed to open camera" << endl;
      return;
  }

  // get an image from the camera
  // query << cap;

  // Hold coordinates of images seen in the following image matching functions
  float leftPostX = 0, rightPostX = 0;

  // Find the gate
  if(checkGate(cap, &leftPostX, &rightPostX)){
    kb->Pillar1Seen = true;
    kb->Pillar2Seen = true;
    kb->Pillar1X = leftPostX;
    kb->Pillar2X = rightPostX;
  }

  /*
  if(match(query, "sword"))
    kb->swordSeen = true;

  if(match(query, "shield"))
    kb->shieldSeen = true;
    */

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
