#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int main( int argc, char** argv ) {
   CvCapture* capture0;
   CvCapture* capture1;

   capture0 = cvCreateCameraCapture(0);
   capture1 = cvCreateCameraCapture(1);

   assert( capture0 != NULL );
   assert( capture1 != NULL );

   IplImage* bgr_frame0 = cvQueryFrame( capture0 );
   IplImage* bgr_frame1 = cvQueryFrame( capture1 );

   CvSize size0 = cvSize(
                        (int)cvGetCaptureProperty( capture0,
                                                  CV_CAP_PROP_FRAME_WIDTH),
                        (int)cvGetCaptureProperty( capture0,
                                                  CV_CAP_PROP_FRAME_HEIGHT)
                        );
   CvSize size1 = cvSize(
                        (int)cvGetCaptureProperty( capture1,
                                                  CV_CAP_PROP_FRAME_WIDTH),
                        (int)cvGetCaptureProperty( capture1,
                                                  CV_CAP_PROP_FRAME_HEIGHT)
                        );

   //cvNamedWindow( "Webcam", CV_WINDOW_AUTOSIZE );

   CvVideoWriter *writer0 = cvCreateVideoWriter("/home/robosub/vid0.AVI",
                                               CV_FOURCC('D','I','V','X'),
                                               30,
                                               size0
                                               );
   CvVideoWriter *writer1 = cvCreateVideoWriter("/home/robosub/vid1.AVI",
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
       //char c = cvWaitKey( 33 );
       //if( c == 27 ) break;
   }
   cvReleaseVideoWriter( &writer0 );
   cvReleaseVideoWriter( &writer1 );
   cvReleaseCapture( &capture0 );
   cvReleaseCapture( &capture1 );
   //cvDestroyWindow( "Webcam" );
   return( 0 );
}
