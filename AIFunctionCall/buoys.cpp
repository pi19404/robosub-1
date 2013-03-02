/* Kenneth Perrault
   Buoy targeting for AUS device
   Goal: capture and image and scan it for different color circles.
         modify an array of buoys (passed by reference) to indicate the
         location and color of buoys.
         return the number of buoys found and also save the number of buoys
         found in an integer passed by reference.
   Preconditions: Buoy bump task starts.
   Postconditions: Return to orange guide rail.
*/


#include <time.h>
#include <cstdio>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define DILATE_ERODE_SCALE 3
#define USE_CONTROLS false

using namespace cv;
using namespace std;

struct OFFSETDATA {
	int greenHueHigh;
	int greenHueLow;
	int redHueHigh;
	int redHueLow;
	int yellowHueHigh;
	int yellowHueLow;
	int greenSatHigh;
	int greenSatLow;
	int redSatHigh;
	int redSatLow;
	int yellowSatHigh;
	int yellowSatLow;
	int greenValueHigh;
	int greenValueLow;
	int redValueHigh;
	int redValueLow;
	int yellowValueHigh;
	int yellowValueLow;		
} offsetData;

struct HARDDATA {
	int greenHueHigh;
	int greenHueLow;
	int redHueHigh;
	int redHueLow;
	int yellowHueHigh;
	int yellowHueLow;
	int greenSatHigh;
	int greenSatLow;
	int redSatHigh;
	int redSatLow;
	int yellowSatHigh;
	int yellowSatLow;
	int greenValueHigh;
	int greenValueLow;
	int redValueHigh;
	int redValueLow;
	int yellowValueHigh;
	int yellowValueLow;		
} hardData;

using namespace cv;

int checkBuoys(Buoy buoys[3], int *numFound)
{
  VideoCapture cap(0);

  // make sure the camera is functioning
  if(!cap.isOpened())
    return -1;

  // INITIALIZE PRESET DATA
  /*offsetData.greenHueHigh = 38;
  offsetData.greenHueLow = 12;
  offsetData.greenSatHigh = 50;
  offsetData.greenSatLow = -77;
  offsetData.greenValueHigh = 50;
  offsetData.greenValueLow = -50;
  offsetData.redHueHigh = 17;
  offsetData.redHueLow = -5;
  offsetData.redSatHigh = 50;
  offsetData.redSatLow = -65;
  offsetData.redValueHigh = 65;
  offsetData.redValueLow = -30;
  offsetData.yellowHueHigh = 22;
  offsetData.yellowHueLow = 3;
  offsetData.yellowSatHigh = 70;
  offsetData.yellowSatLow = -60;
  offsetData.yellowValueHigh = 60;
  offsetData.yellowValueLow = -10;
  */

  // Below water
  hardData.greenHueHigh = 82;
  hardData.greenHueLow = 43;
  hardData.greenSatHigh = 100;
  hardData.greenSatLow = 44;
  hardData.greenValueHigh = 80;
  hardData.greenValueLow = 28;
  hardData.redHueHigh = 256;
  hardData.redHueLow = 242;
  hardData.redSatHigh = 95;
  hardData.redSatLow = 38;
  hardData.redValueHigh = 100;
  hardData.redValueLow = 0;
  hardData.yellowHueHigh = 45;
  hardData.yellowHueLow = 21;
  hardData.yellowSatHigh = 100;
  hardData.yellowSatLow = 31;
  hardData.yellowValueHigh = 94;
  hardData.yellowValueLow = 22;



  int modeCount = 1;

  // will be modified for each color preset
  int hueLow = 0, hueHigh = 0, satLow = 0, satHigh = 0, valueLow = 0, valueHigh = 0;
  double minVal = 0;
  double maxVal = 0;
  Point minLoc;
  Point maxLoc;
  int buoyCount = 0;

  Mat frame; //Source frame
  Mat hsv;   //Converted to HSV
  Mat mask;  //Binary mask
  Mat blurMask; //fscale, blurred mask
  Mat nullMat;
  Point nullPoint(-1,-1);

  // get an image and convert it to its hue saturation values
  cap >> frame;
  cvtColor( frame, hsv, CV_BGR2HSV );

#ifdef DEBUG
  namedWindow("Camera Display", CV_WINDOW_AUTOSIZE);
  imshow("Camera Display", frame);
  waitKey(0);

  // to display the mask later
  namedWindow("mask", CV_WINDOW_AUTOSIZE);
#endif


  for(modeCount = 1; modeCount <= 3; modeCount++)
  {
    switch (modeCount)
    {
      case 1: 
      hueLow = hardData.greenHueLow;
      hueHigh = hardData.greenHueHigh;
      satLow = hardData.greenSatLow;
      satHigh = hardData.greenSatHigh;
      valueLow = hardData.greenValueLow;
      valueHigh = hardData.greenValueHigh;
      break;

      case 2:
      hueLow = hardData.redHueLow;
      hueHigh = hardData.redHueHigh;
      satLow = hardData.redSatLow;
      satHigh = hardData.redSatHigh;
      valueLow = hardData.redValueLow;
      valueHigh = hardData.redValueHigh;
      break;

      case 3:
      hueLow = hardData.yellowHueLow;
      hueHigh = hardData.yellowHueHigh;
      satLow = hardData.yellowSatLow;
      satHigh = hardData.yellowSatHigh;
      valueLow = hardData.yellowValueLow;
      valueHigh = hardData.yellowValueHigh;
      break;
    }

    // CREATE MASK
    inRange( hsv,
             Scalar((hueLow/360.0)*255, (satLow/100.0)*255, (valueLow/100.0)*255, 0),
             Scalar((hueHigh/360.0)*255,(satHigh/100.0)*255, (valueHigh/100.0)*255, 0),
             mask );
    dilate(mask,mask,nullMat,nullPoint, DILATE_ERODE_SCALE);
    erode(mask,mask,nullMat,nullPoint, DILATE_ERODE_SCALE); 

#ifdef DEBUG
  imshow("mask", mask);
  waitKey(0);
#endif

    // INTERPRET MASK
    minMaxLoc(mask, &minVal, &maxVal, &minLoc, &maxLoc);
    GaussianBlur( mask, blurMask, Size(9,9), 2, 2);

    // get a vector of all the circles in the image
    vector<Vec3f> circles;
    HoughCircles(blurMask, circles, CV_HOUGH_GRADIENT, 2, blurMask.rows/2, 200, 100, 50, 640);

    // loop through every circle in the image
    for( size_t i = 0; i < circles.size(); i++ )
    {
      // get the center point and radius of the current circle
      int tempCenterX = circles[i][0];
      int tempCenterY = circles[i][1];
      Point tempCenter(cvRound(circles[i][0]), cvRound(circles[i][1]));

      switch (modeCount)
      {
        case 1: // green buoys
          buoys[buoyCount].buoySeen = true;
          buoys[buoyCount].buoyX = tempCenterX;
          buoys[buoyCount].buoyY = tempCenterY;
          buoys[buoyCount].buoyZ = 0; // TODO: solve for z
          buoys[buoyCount].buoyColor = GREEN;

          // don't increment past the array bounds (would cause a seg fault)
          if(buoyCount < 3){
            buoyCount++;
          }
          else{
            break;
          }
        break;
        case 2: // red buoys
          buoys[buoyCount].buoySeen = true;
          buoys[buoyCount].buoyX = tempCenterX;
          buoys[buoyCount].buoyY = tempCenterY;
          buoys[buoyCount].buoyZ = 0; // TODO: solve for z
          buoys[buoyCount].buoyColor = RED;
          if(buoyCount < 3){
            buoyCount++;
          }
          else{
            break;
          }
        break;
        case 3: // yellow buoys
          buoys[buoyCount].buoySeen = true;
          buoys[buoyCount].buoyX = tempCenterX;
          buoys[buoyCount].buoyY = tempCenterY;
          buoys[buoyCount].buoyZ = 0; // TODO: solve for z
          buoys[buoyCount].buoyColor = YELLOW;
          if(buoyCount < 3){
            buoyCount++;
          }
          else{
            break;
          }
        break;
      }
    }

    // no need to keep going if all buoys have been seen
    if(buoyCount == 3)
      break;
  }

  cap.release();
  *numFound = buoyCount;
  return buoyCount;
}


