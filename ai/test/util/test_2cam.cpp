#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

int main( int argc, char** argv ) {
   CvCapture* capture0;
   CvCapture* capture1;
   char filepath0[256];
   char filepath1[256];

   if (argc < 3)
   {
      fprintf(stderr, "You must give vid0 and vid1 output filenames\n");
      return( 1 );
   }

   strcpy(filepath0, "/home/robosub/");
   strcat(filepath0, argv[1]);

   strcpy(filepath1, "/home/robosub/");
   strcat(filepath1, argv[2]);

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

   capture1 = cvCreateCameraCapture(1);
   assert( capture1 != NULL );
   IplImage* bgr_frame1 = cvQueryFrame( capture1 );
   CvSize size1 = cvSize(
                        (int)cvGetCaptureProperty( capture1,
                                                  CV_CAP_PROP_FRAME_WIDTH),
                        (int)cvGetCaptureProperty( capture1,
                                                  CV_CAP_PROP_FRAME_HEIGHT)
                        );
   CvVideoWriter *writer1 = cvCreateVideoWriter(filepath1,
                                               CV_FOURCC('D','I','V','X'),
                                               30,
                                               size1
                                               );

   while( ((bgr_frame0 = cvQueryFrame( capture0 )) != NULL) &&
           (bgr_frame1 = cvQueryFrame( capture1 )) != NULL)
   {
       cvWriteFrame(writer0, bgr_frame0 );
       cvWriteFrame(writer1, bgr_frame1 );
       //cvShowImage( "Webcam", bgr_frame );
       char c = cvWaitKey( 33 );
       if( c == 27 ) break;
   }
   cvReleaseVideoWriter( &writer0 );
   cvReleaseVideoWriter( &writer1 );
   cvReleaseCapture( &capture0 );
   cvReleaseCapture( &capture1 );
   //cvDestroyWindow( "Webcam" );
   return( 0 );
}
