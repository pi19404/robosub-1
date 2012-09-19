/* Ian Meharg
   Round Object Searching for AUS device
   Goal: Identify an object of given color (text),
		 and track the roundest of the bunch.
   Preconditions: Related task starts.
   Exit condition: Killed by main AI.
   Postconditions: Return to orange guide rail.
*/


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
vColor validifyColor (std::string inputColor) { //requires global vColorStr
	int i=0;
	while (vColorStr[i] != "vC_error") {
		if (!inputColor.find(vColorStr[i])) {
			return (vColor) i;
		}
		i++;
	}
	return vC_error;
}

//DEBUGGING FUNCTIONS
std::ofstream global_logger;
void echo(std::string message) {
	if (VISUAL_DEBUG) std::cerr << message << std::endl;
	else global_logger << message << std::endl;
}
void echo(std::string message, int number) {
	if (VISUAL_DEBUG) std::cerr << message << ": " << number << std::endl;
	else global_logger << message << std::endl;
}

//SORTING FUNCTIONS
bool compareRectArea (Rect first, Rect second)
{
	if ((first.width * first.height) >
		(second.width*second.height)) return true;
	return false;
}

//BOUNDING FUNCTIONS
std::list<Rect> getSortedRectangles(Mat &fromBinaryImage) //adopted from David's match program
{
    //Mat edgeDetect;
    //Canny(fromImage, edgeDetect, 50, 200, 3);
	std::list<Rect> rectangles;
	Rect tempRect;
    vector<vector<Point> > contours;
    
    Mat imgCopy;
    fromBinaryImage.copyTo(imgCopy);

    findContours(imgCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	//echo("Number of contour arrays", contours.size());

    for (unsigned int i=0; i<contours.size(); i++) {
		tempRect = boundingRect(Mat(contours[i]));
		rectangles.push_front(tempRect);
    }
	rectangles.sort(compareRectArea);
	return rectangles;    
}

void getBestCircle(Mat &fromBinaryImage, Point * center, float * radius)
{
	vector<Vec3f> circles;
	*center = Point(0,0);
	*radius = 0;
	HoughCircles(fromBinaryImage, circles, CV_HOUGH_GRADIENT, 2, fromBinaryImage.rows/2, 200, 100, 50, 640);
	if (circles.size() > 0) {
		*center = Point(circles[0][0], circles[0][1]);
		*radius = circles[0][2];
	}
}
int isRectInCorner(Rect rectInQuestion, int midpointX, int midpointY) {
	if (rectInQuestion.x<=1) {
		if (rectInQuestion.y <= 1) return 1;
		else if (rectInQuestion.y+(rectInQuestion.height) == (midpointY*2)-1) return 2;
	}
	else if (rectInQuestion.x+rectInQuestion.width >= midpointX*2-1) {
		if (rectInQuestion.y <= 1) return 3;
		else if (rectInQuestion.y+(rectInQuestion.height) == (midpointY*2)-1) return 4;
	}
	return 0;
}



// FORMULAS
float getPercentOfScreen(float frameWidth, float objectWidth) {
	return objectWidth/frameWidth;
}
float approxDistance(float percentOfScreen, float targetWidthCM)
{
	return ((1.0375870127 /
			(percentOfScreen * 0.2663492405)) *
			(30.48 / targetWidthCM / 10.7))*10;
}
float approxRadX(float fov, int midpointX, int objX)
{
	if (objX >= midpointX)
		return ((fov/2) * objX/(midpointX*2));
	else
		return ((fov/2) * objX/midpointX)*-1;
}
float approxCoordX(float distance, float radX) {
	return (tan(radX) * distance);
}

int findGate()
{
	//OPEN FILE
	global_logger.open("postlog.txt");
	
	
	//VALIDIFY COLOR
	vColor lookForColor = orange;
	echo("Initializing... (orange post finder)");
	/*echo("Argc", argc);
	std::string colorInput = argv[1];
	echo(colorInput);
	lookForColor = validifyColor(colorInput);
	echo("Identified color", lookForColor);
	if (lookForColor == vC_error) {
		std::cout << "MSG: Incorrect program parameters. Format is 'giro blue'" << std::endl;
		exit(1);
	}
	vColor lookForColor = vColor ((int)lookForColor+1);
	if (lookForColor == vC_error) lookForColor = (vColor) 0;*/

	//INITIALIZE COLOR VALUES BASED ON VALIDIFIED NUMBER
	hardDataStruct hardData;
	initColorPresets(&hardData);
	echo("hardData.hue[low][lookForColor]", hardData.hue[low][lookForColor]);
	echo("hardData.hue[high][lookForColor]", hardData.hue[high][lookForColor]);
	echo("hardData.sat[low][lookForColor]", hardData.sat[low][lookForColor]);
	echo("hardData.sat[high][lookForColor]", hardData.sat[high][lookForColor]);
	echo("hardData.value[low][lookForColor]", hardData.value[low][lookForColor]);
	echo("hardData.value[high][lookForColor]", hardData.value[high][lookForColor]);

	//SCOPED VARIABLE LIST
	double frameCount = 0;
	time_t start = time(NULL);

	Mat frame; //Source frame
	Mat hsv;   //Converted to HSV
	Mat mask;  //Binary mask
	Mat blurMask; //fscale, blurred mask
	Mat nullMat;
	Point nullPoint(-1,-1);

	VideoCapture cap(0); // open the default camera
	Mat orig_frame;

	if(!cap.isOpened()) {
		echo("Could not open camera stream!");
		exit(1);
	}
	int midpointX = cap.get(CV_CAP_PROP_FRAME_WIDTH)/2;  //Precalculate midpoint
	int midpointY = cap.get(CV_CAP_PROP_FRAME_HEIGHT)/2;

	// CRITICAL OBJECT VARIABLES
	Rect objRect(0,0,0,0);
	Rect objRect2(0,0,0,0);
	Rect objPostBox(0,0,0,0);
	objStruct giroObject;

	// CAMERA INPUT
	if (STATIC_DEBUG) {
		orig_frame = imread(STATIC_IMAGE, CV_LOAD_IMAGE_COLOR);
		midpointX = orig_frame.cols/2;  //Precalculate midpoint
		midpointY = orig_frame.rows/2;
	}

	// WINDOW OUTPUT
	namedWindow("Camera Display", CV_WINDOW_AUTOSIZE);
	namedWindow("Mask", CV_WINDOW_AUTOSIZE);

	while (1) {
		// CAPTURE AND CONVERT FRAME
		if (!STATIC_DEBUG) cap >> frame;
		else orig_frame.copyTo(frame);

		cvtColor( frame, hsv, CV_BGR2HSV );

		// CALCULATIONS
		Vec3b colorHSV = hsv.at<Vec3b>( //HSV for everything else
			midpointY,
			midpointX);

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

		// INTERPRET MASK
		//void minMaxLoc(const SparseMat& src, double* minVal, double* maxVal, int* minIdx=0, int* maxIdx=0)
		std::list<Rect> rectangles = getSortedRectangles(blurMask);		
		objRect = rectangles.front();
		rectangles.pop_front();
		if (objRect.width > objRect.height*1.2)
		{
			objRect = rectangles.front();
					rectangles.pop_front();
		}
		objRect2 = rectangles.front();
				rectangles.pop_front();
		// JUDGE RESULTS
			//Situation 1: Circle and Rectangle match up.
		float percentOfScreen;
		float xRad;
		float yScale;

		echo("objRect.x",objRect.x);
		echo("objRect2.x",objRect2.x);

		if (objRect != objRect2) {
			if (objRect.x < objRect2.x) {
				objPostBox = objRect;
				objPostBox.width = objRect2.x + objRect2.width - objRect.x;
			} else {
				objPostBox = objRect2;
				objPostBox.width = objRect.x + objRect.width - objRect2.x;
			}
			
			//echo("objRect", objRect.x);
			//echo("objPostBox.x",objPostBox.x);
			percentOfScreen = getPercentOfScreen(midpointX*2, objPostBox.width/2);
			giroObject.z_cm = approxDistance(percentOfScreen, BUOY_WIDTH);
			xRad = approxRadX(FOV, midpointX, objPostBox.x+(objPostBox.width/2));
			giroObject.x_cm = approxCoordX(giroObject.z_cm, xRad);
			yScale = (giroObject.x_cm)/(objRect.x+(objRect.width/2) - midpointX);
			giroObject.y_cm = (objPostBox.y+(objPostBox.height/2) - midpointY)*(yScale)*-1;
			giroObject.confirmation = SQUARE_CH;
		}
		else
		{
			giroObject.x_cm = 0;
			giroObject.y_cm = 0;
			giroObject.z_cm = 0;
			giroObject.confirmation = '?';
		}

		// DRAW RESULTS
			//void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
			//void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
		if (VISUAL_DEBUG) {
			// DRAW RETICLE
			rectangle ( frame, Point(-1,-1), Point(midpointX, midpointY), vColorBGR[lookForColor],1,1,0);
			rectangle ( frame, Point(midpointX*2+1,midpointY*2+1), Point(midpointX, midpointY), vColorBGR[lookForColor],1,1,0);
			// DRAW BOUNDS
			Point rectP1;
			rectP1.x = objPostBox.x;
			rectP1.y = objPostBox.y;
			Point rectP2;
			rectP2.x = objPostBox.x + objPostBox.width;
			rectP2.y = objPostBox.y + objPostBox.height;
			//echo("rectP1.x", rectP1.x);
			rectangle(frame, rectP1, rectP2, vColorBGR[lookForColor]+OFF_SCALAR, 2, 8, 0);
		}
		

		printf("<post_target, %s, %lf, %lf, %lf, %c>\n",
			vColorStr[lookForColor].c_str(),
			giroObject.x_cm,
			giroObject.y_cm,
			giroObject.z_cm, 
			giroObject.confirmation);
		global_logger << "<post_target, " << vColorStr[lookForColor]
				<< ", " << giroObject.x_cm
				<< ", " << giroObject.y_cm
				<< ", " << giroObject.z_cm
				<< ", " << giroObject.confirmation << ">\n";

		// UPDATE WINDOWS
		if (VISUAL_DEBUG) {
			imshow("Camera Display", frame);
			imshow("Mask", blurMask);
		}

		// ON ANY KEY
		char keyPress = waitKey(1);
		if (keyPress == 'c' || keyPress == 'd')
			echo("No functionality.");
		else if (keyPress >= 0)
			break;
        frameCount++;		
	}
	time_t end = time(NULL);
	echo("FPS", (frameCount) / ((double)(end - start)));
	global_logger.close();
	exit(0);
}
