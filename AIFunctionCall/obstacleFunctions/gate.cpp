/* Kenneth Perrault
 * This file contains the function checkGate().
 * checkGate() is used to identify whether or not the gate obstacle
 * is in the cameras field of vision.
 *
 * Parameters:
 * - float *leftPostX
 * - float *rightPostX
 * - float *z_distance
 *
 * Return value:
 * Bool
 *
 * Description:
 * If the gate obstacle is identified, the input parameters are all
 * modified to indicate the position of the gate and true is returned.
 * If the gate is not seen, the input parameters are not modified and
 * false is returned.
 *
*/

#define ABOVE_WATER true
#define NUM_OF_COLORS 6 //this includes "vC_error"

#define DILATE_ERODE_SCALE 3
#define GREEN_SCALAR Scalar(0,255,0)
#define RED_SCALAR Scalar(0,0,255)
#define YELLOW_SCALAR Scalar(0,255,255)
#define BLUE_SCALAR Scalar(255,0,0)
#define ORANGE_SCALAR Scalar(0,200,255)
#define WHITE_SCALAR Scalar(255,255,255)

#define FOV 1.09 //radians
#define GATE_WIDTH 10

//PRESET FUNCTIONS
typedef enum {red, green, blue, yellow, orange, vC_error} vColor; //for "verified color"
typedef enum {low, high} vLevel;						//for "varied level"
typedef struct HARDDATA_GATE {
	int hue[2][NUM_OF_COLORS];
	int sat[2][NUM_OF_COLORS];
	int value[2][NUM_OF_COLORS];
} hardDataStruct;

void initColorPresets (hardDataStruct * hardData) {
	if (ABOVE_WATER) {	
		hardData->hue[high][green] = 107;
		hardData->hue[low][green] = 71;
		hardData->sat[high][green] = 100;
		hardData->sat[low][green] = 23;
		hardData->value[high][green] = 100;
		hardData->value[low][green] = 30;
		hardData->hue[high][red] = 28;
		hardData->hue[low][red] = 0;
		hardData->sat[high][red] = 100;
		hardData->sat[low][red] = 38;
		hardData->value[high][red] = 100;
		hardData->value[low][red] = 0;
		hardData->hue[high][yellow] = 50;
		hardData->hue[low][yellow] = 38;
		hardData->sat[high][yellow] = 100;
		hardData->sat[low][yellow] = 40;
		hardData->value[high][yellow] = 100;
		hardData->value[low][yellow] = 28;
		hardData->hue[high][orange] = 25;
		hardData->hue[low][orange] = 10;
		hardData->sat[high][orange] = 100;
		hardData->sat[low][orange] = 43;
		hardData->value[high][orange] = 100;
		hardData->value[low][orange] = 34;
	} else {
		// Below water
		hardData->hue[high][green] = 82;
		hardData->hue[low][green] = 43;
		hardData->sat[high][green] = 100;
		hardData->sat[low][green] = 44;
		hardData->value[high][green] = 80;
		hardData->value[low][green] = 28;
		hardData->hue[high][red] = 256;
		hardData->hue[low][red] = 242;
		hardData->sat[high][red] = 95;
		hardData->sat[low][red] = 38;
		hardData->value[high][red] = 100;
		hardData->value[low][red] = 0;
		hardData->hue[high][yellow] = 45;
		hardData->hue[low][yellow] = 21;
		hardData->sat[high][yellow] = 100;
		hardData->sat[low][yellow] = 31;
		hardData->value[high][yellow] = 94;
		hardData->value[low][yellow] = 22;
		hardData->hue[high][orange] = 25;
		hardData->hue[low][orange] = 10;
		hardData->sat[high][orange] = 100;
		hardData->sat[low][orange] = 43;
		hardData->value[high][orange] = 100;
		hardData->value[low][orange] = 34;
	}
}


// used to calculate distance
float getPercentOfScreen(float frameWidth, float objectWidth) {
	return objectWidth/frameWidth;
}
float approxDistance(float percentOfScreen, float targetWidthCM)
{
	return ((1.0375870127 /
			(percentOfScreen * 0.2663492405)) *
			(30.48 / targetWidthCM / 10.7))*10;
}


bool checkGate(float *leftPostX, float *rightPostX, float *z_distance)
{
  VideoCapture cap(0);

  // make sure the camera is functioning
  if(!cap.isOpened())
  {
    printf("here\n");
    return 0; 
  }

  bool foundGate = false;

  //VALIDIFY COLOR
  vColor lookForColor = orange;

  //INITIALIZE COLOR VALUES BASED ON VALIDIFIED NUMBER
  hardDataStruct hardData;
  initColorPresets(&hardData);

  Mat frame; //Source frame
  Mat hsv;   //Converted to HSV
  Mat mask;  //Binary mask
  Mat blurMask; //fscale, blurred mask
  Mat nullMat;
  Point nullPoint(-1,-1);

  int midpointX = cap.get(CV_CAP_PROP_FRAME_WIDTH)/2;  //Precalculate midpoint

  // CRITICAL OBJECT VARIABLES
  Rect objRect(0,0,0,0);
  Rect objRect2(0,0,0,0);
  Rect objPostBox(0,0,0,0);


  // Capture the frame and save a copy of it
  cap >> frame;

#ifdef DEBUG
  namedWindow("Camera", CV_WINDOW_AUTOSIZE);
  imshow("Camera", frame);
  waitKey(0);
#endif

  cvtColor( frame, hsv, CV_BGR2HSV );

  // CALCULATIONS
  // Stores the hue saturation values at the center of the image
  // never really used but possibly useful
  /*
  Vec3b colorHSV = hsv.at<Vec3b>( //HSV for everything else
      midpointY,
      midpointX);
  */

  // CREATE MASK
  inRange( hsv,
           Scalar((hardData.hue[low][lookForColor]/360.0)*255,
              (hardData.sat[low][lookForColor]/100.0)*255,
              (hardData.value[low][lookForColor]/100.0)*255, 0),
           Scalar((hardData.hue[high][lookForColor]/360.0)*255,
              (hardData.sat[high][lookForColor]/100.0)*255,
              (hardData.value[high][lookForColor]/100.0)*255, 0),
           mask );
  dilate(mask,mask,nullMat,nullPoint, 1*(DILATE_ERODE_SCALE)); //The calibrateMode*10 expression reduces strain on the processor.
  erode(mask,mask,nullMat,nullPoint, 1*(DILATE_ERODE_SCALE)); 
  GaussianBlur( mask, blurMask, Size(9,9), 2, 2);		

#ifdef DEBUG
  imshow("mask", mask);
  waitKey(0);
#endif

  // INTERPRET MASK
  //void minMaxLoc(const SparseMat& src, double* minVal, double* maxVal, int* minIdx=0, int* maxIdx=0)
  std::list<Rect> rectangles = getSortedRectangles(blurMask);		

  // must have seen exactly two rectangles
  if(rectangles.size() >= 2){
    objRect = rectangles.front();
    rectangles.pop_front();
    objRect2 = rectangles.front();
    rectangles.pop_front();

  // JUDGE RESULTS
    if (objRect.x < objRect2.x) {
      // makes a rectangle that outlines the path through the gate
      // used to calculate its distance
      objPostBox = objRect;
      objPostBox.width = objRect2.x + objRect2.width - objRect.x;
      *leftPostX  = objRect.x;
      *rightPostX = objRect2.x;

    } else {       
      // makes a rectangle that outlines the path through the gate
      objPostBox = objRect2;
      objPostBox.width = objRect.x + objRect.width - objRect2.x;
      *leftPostX  = objRect2.x;
      *rightPostX = objRect.x;
    }
    float percentOfScreen = getPercentOfScreen(midpointX*2, objPostBox.width/2);
    printf("width: %d\n", objPostBox.width);
    *z_distance = approxDistance(percentOfScreen, /*GATE_WIDTH*/ objPostBox.width);
	
    foundGate = true;
  }
  else
  {
      foundGate = false;
  }

  cap.release();

  return foundGate;
}
