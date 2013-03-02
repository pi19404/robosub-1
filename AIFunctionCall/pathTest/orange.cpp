/* This program is a test built around the following orange strips of tape on 
   the floor of the pool. We will need to identify the orange strips and find
   the angle of the strip realative to the sub.

   Based on example here:
   http://wiki.elphel.com/index.php?title=OpenCV_Tennis_balls_recognizing_tutorial

   * Convert the image from BGR to HSV.
   * Create a mask of pixels that are orange.
   * Fill in gaps in mask
   * Edge detection
   * Look for lines
*/

#include <time.h>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <list>

#include <opencv2/opencv.hpp>

int hueLow = 10;
int hueHigh = 25;
int satLow = 43;
int satHigh = 100;
int valueLow = 54;
int valueHigh = 100;

using namespace cv;

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

    for (unsigned int i=0; i<contours.size(); i++) {
		tempRect = boundingRect(Mat(contours[i]));
		rectangles.push_front(tempRect);
    }
	rectangles.sort(compareRectArea);
	return rectangles;    
}

int main(int argc, char **argv)
{
  /* Get the first camera */
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  /* Create a window to use for displaying the images */
  namedWindow("one", CV_WINDOW_AUTOSIZE);
  createTrackbar("Hue low", "one", &hueLow, 360);
  createTrackbar("Hue high", "one", &hueHigh, 360);
  createTrackbar("Sat low", "one", &satLow, 100);
  createTrackbar("Sat high", "one", &satHigh, 100);
  createTrackbar("Value low", "one", &valueLow, 100);
  createTrackbar("Value high", "one", &valueHigh, 100);

  namedWindow("two", CV_WINDOW_AUTOSIZE);
  namedWindow("three", CV_WINDOW_AUTOSIZE);
  namedWindow("crop", CV_WINDOW_AUTOSIZE);

  unsigned int frames = 0;
  time_t start = time(NULL);

  Mat frame;
  Mat image;
  Mat hsv;
  Mat mask;
  Mat lineDetect;
  Mat nullMat;
  Mat crop;
  Point nullPoint(-1,-1);
  while (1) 
  {
    // Get an image from the camera
    cap >> frame;
    //GaussianBlur( frame, image, Size( 31, 31 ), 0, 0 );
    cvtColor( frame, hsv, CV_BGR2HSV ); //convert to hsv colors
    // http://stackoverflow.com/questions/8753833/exact-skin-color-hsv-range
    // http://www.yafla.com/yaflaColor/ColorRGBHSL.aspx
    // This should match orange things
    inRange( hsv,
             Scalar((hueLow/360.0)*255, (satLow/100.0)*255, (valueLow/100.0)*255, 0),
             Scalar((hueHigh/360.0)*255,(satHigh/100.0)*255, (valueHigh/100.0)*255, 0),
             mask );
    dilate(mask,mask,nullMat,nullPoint, 1);
    erode(mask,mask,nullMat,nullPoint, 1);
    medianBlur(mask,mask,1);
    std::list<Rect> rectangles = getSortedRectangles(mask);		
    //GaussianBlur( mask, blurMask, Size(9,9), 2, 2);		

    // OpenCV-2.3.1/samples/cpp/houghlines.cpp crashed (after copy pasta)
    // for some reason.

    // I used the example in the docs for HoughLinesP.
    
    Rect path = rectangles.front();
    if(rectangles.size() > 1)
    {
      rectangles.pop_front();
      Rect tmp = rectangles.front();
      if(tmp.x > path.x) // always choose the right path
        path = tmp;

    }


      crop = mask(path);
      Canny(crop, lineDetect, 50, 200, 3);
      vector<Vec4i> lines;
      HoughLinesP( lineDetect, lines,
                 1,         // rho
                 CV_PI/180, // theta
                 40,        // threshhold
                 10,        // min-length
                 10         // max-line-gap
                );
    

      
      for( size_t i = 0; i < lines.size(); i++ )
      {
          line( crop, Point(lines[i][0], lines[i][1]),
            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
      }


    //http://stackoverflow.com/questions/491738/how-do-you-calculate-the-average-of-a-set-of-angles
    Point sum(0,0);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Point p = Point(lines[i][0],lines[i][1]) - 
                  Point(lines[i][2],lines[i][3]);
        // all vectors should point up
        if ( p.y < 0 )
            p *= -1;
        sum += p;
    }
    double angle = atan( ((double)sum.y) / sum.x );
    double angleDegrees = (angle * 180) / CV_PI;
    if(angleDegrees < 0)
      angleDegrees = 90 + angleDegrees;
    else
      angleDegrees = -1 * (90 - angleDegrees);


    char angleStr[100];
    snprintf( angleStr, 100, "ANGLE: %f", angleDegrees );
    putText( frame, angleStr, Point(50, 50),
             FONT_HERSHEY_SIMPLEX,      //font
             1,                         //scale
             Scalar( 0, 0, 255, 0) );   //BGRA

    // TODO: find center
    // http://www.aishack.in/2010/07/tracking-colored-objects-in-opencv/
    // http://www.aishack.in/2011/06/image-moments/


    imshow("one", mask);
    imshow("two", frame);
    imshow("three", lineDetect);
    imshow("crop", crop);
    //Size size = hsv.size();
    if(waitKey(1) >= 0) break;
    frames++;
  }
  time_t end = time(NULL);
  printf("FPS: %f\n", ((double)frames) / ((double)(end - start)));   
}
