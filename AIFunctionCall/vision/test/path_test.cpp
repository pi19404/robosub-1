#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "path.h"
//#include "path.cpp"

using namespace cv;

int
main(int argc, char** argv)
{
   float leftPostX = 0.0;
   float rightPostX = 0.0;
   float z_distance = 0.0;
   double angleDegrees = 0.0;
   vector<vector<Point> > squares;
   const char *wndname = "pathtestwindow";
   namedWindow(wndname, CV_WINDOW_AUTOSIZE);
   namedWindow("frame", CV_WINDOW_AUTOSIZE);
   namedWindow("after", CV_WINDOW_AUTOSIZE);
   VideoCapture cap("/home/cevans/Videos/Obstacle_Clips/pathOutOfStartGate640x480.mp4");
   if (!cap.isOpened())
   {
      printf("video didn't open\n");
      return( 1 );
   }
   Mat image;
   while(true)
   {
      //printf("checking again\n");
     // cap >> image;
      //imshow(wndname, image);
      //checkGate(&leftPostX, &rightPostX, &z_distance, cap);
      checkPath(&angleDegrees, cap);

      printf("heading is %f\n", angleDegrees);
      int c = waitKey();
   }
   cap.release();
}
