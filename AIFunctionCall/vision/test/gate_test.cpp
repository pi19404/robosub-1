#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "gate.h"
#include "squares.cpp"

using namespace cv;

int
main(int argc, char** argv)
{
   float leftPostX = 0.0;
   float rightPostX = 0.0;
   float z_distance = 0.0;
   vector<vector<Point> > squares;
   const char *wndname = "startgatetest";
   namedWindow(wndname, CV_WINDOW_AUTOSIZE);
   //CvCapture *capture = cvCreateFileCapture("/home/cevans/Videos/Obstacle_Clips/startGate640x480.mp4");
   VideoCapture cap("/home/cevans/Videos/Obstacle_Clips/startGate640x480.mp4");
   if (!cap.isOpened())
   {
      printf("video didn't open\n");
      return( 1 );
   }
   Mat image;
   while(true)
   {
      printf("checking again\n");
      cap >> image;
      //checkGate(&leftPostX, &rightPostX, &z_distance, cap);
      findSquares(image, squares);
      drawSquares(image, squares, wndname);
      int c = waitKey();
   }
   cap.release();
}
