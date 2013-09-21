#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

int main( int argc, char** argv ) {
   CvCapture* capture0;
   char filepath0[256];
   if (argc < 2)
   {
      fprintf(stderr, "You must give a vid0 output filename\n");
      return( 1 );
   }
   strcpy(filepath0, "/home/robosub/");
   strcat(filepath0, argv[1]);

   capture0 = cvCreateCameraCapture(0);
   assert( capture0 != NULL );
   IplImage* bgr_frame0 = cvQueryFrame( capture0 );
   CvSize size0 = cvSize(
                        (int)cvGetCaptureProperty( capture0,
                                                  CV_CAP_PROP_FRAME_WIDTH),
                        (int)cvGetCaptureProperty( capture0,
                                                  CV_CAP_PROP_FRAME_HEIGHT)
                        );
   CvVideoWriter *writer0 = cvCreateVideoWriter(filepath0,
                                               CV_FOURCC('D','I','V','X'),
                                               30,
                                               size0
                                               );


   while( ((bgr_frame0 = cvQueryFrame( capture0 )) != NULL))
   {
       cvWriteFrame(writer0, bgr_frame0 );
       //char c = cvWaitKey( 33 );
       //if( c == 27 ) break;
   }
   cvReleaseVideoWriter( &writer0 );
   cvReleaseCapture( &capture0 );
   return( 0 );
}
