/*******************************************************************************
 * David Hawbaker
 * WSU RoboSub
 * Camera Image Matching
 * Created: March 1, 2012
 * Last Updated: November 1, 2012 by Kenneth Perrault
 * 
 * This program is used to search for and find images that appear in the camera
 * based on template images. Then return name, position, scale, and orientation
*******************************************************************************/

/*************************** 
  THINGS ABSOLUTELY NEEDED: 
  * Determine location of detected object
  * Determine Scale of detected object
  * Output location, scale, rotation, and what it is
  * Rotation support
  * test with all obstacles
    * make template images for all obstacles

  SCALE : TEST 
  to calculate the scale:
    determine bounding box resolution
    know physical size to resolution through tests
        determine physical size of what each pixel in camera sees

  ROTATION : TEST
    determine bounding box orientation when best match occurs
        do this by doing calculation for every 15 degrees of template

  OTHER OBSTACLES:
  * Matching: Red and White
  * Torpedos: Blue or Red
  * Path: Orange
  * Buoys: Green, Red, Yellow
  * Obstacle Course: Green
  * Feed the Emperor: Yellow Red
  * Laurel Wreath: Green, Red, Grey/White

  MATCH VS AI:
  * Will AI get color combos and determine what to look for? 
    * Have AI determine what to look for, input that to match
  * Will match know color combos and know what to look for? 

***************************/ 

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>

// for color matching (verify whether each is needed)
#include <time.h>  
#include <cstdio>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace cv;

/* Global Variables */
Mat templ1;
Mat templ2;
Mat templ3;
Mat templ4;
Mat query;
Mat img_display;
Mat source;

String image_window  = "Source Image";
String norm_window   = "Normalized View";
String video_window  = "Video_window";

int hueLow    = 0;
int hueHigh   = 15;
int satLow    = 20;
int satHigh   = 100;
int valueLow  = 20;
int valueHigh = 100;
    
const double FILTER_PERCENT   = .001;
const double SHIELD_THRESHOLD = .175;

double currentContArea;
    
/* Function Headers */
bool MatchingMethod( Mat frame, Mat tmpl, int name );
void colorMatch();
bool shieldSize( Mat frame, Mat templ, int name );
bool Match( Mat& query, Mat& roi );
vector<Rect> findShapes(Mat& mask);
Mat cropTempl(Mat& templ );

/* Main */
int match(Mat query, char *imName)
{
    /* Camera */
    /*VideoCapture cap(0); // open the downward facing camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout << "failed to open camera" << endl;
        return 0;
    }*/
    
    // Create Window
    namedWindow( "Image", CV_WINDOW_AUTOSIZE );
    namedWindow( "mask",  CV_WINDOW_AUTOSIZE );
    
	// Define Matricies 
    //Mat query;
    Mat shield;
    Mat sword;
    Mat net;
    Mat trident;
    
    // Load Images
    shield  = imread( "images/shield.png" , 1);
    sword   = imread( "images/sword.png"  , 1);
    //net     = imread( "images/net2.png"   , 1);
    //trident = imread( "images/trident.png", 1 );
    
    Size    dsize(9.0,9.0);
    
    // Convert templ images to HSV
    // Shield
    cvtColor( shield, shield, CV_BGR2HSV ); //convert to hsv colors
    inRange( shield, Scalar((hueLow/360.0)*255, 
                         (satLow/100.0)*255, 
                         (valueLow/100.0)*255, 0),
                 Scalar((hueHigh/360.0)*255,
                         (satHigh/100.0)*255, 
                         (valueHigh/100.0)*255, 0),
                 shield );
    shield = cropTempl(shield);
    GaussianBlur(shield, shield, dsize, 5.0);
    
    // Sword
    cvtColor( sword, sword, CV_BGR2HSV ); //convert to hsv colors
    inRange( sword, Scalar((hueLow/360.0)*255, 
                         (satLow/100.0)*255, 
                         (valueLow/100.0)*255, 0),
                 Scalar((hueHigh/360.0)*255,
                         (satHigh/100.0)*255, 
                         (valueHigh/100.0)*255, 0),
                 sword );
    sword = cropTempl(sword);
    GaussianBlur(sword, sword, dsize, 5.0);
    
    // Net 
    /*cvtColor( net, net, CV_BGR2HSV ); //convert to hsv colors
    inRange( net, Scalar((hueLow/360.0)*255, 
                         (satLow/100.0)*255, 
                         (valueLow/100.0)*255, 0),
                 Scalar((hueHigh/360.0)*255,
                         (satHigh/100.0)*255, 
                         (valueHigh/100.0)*255, 0),
                 net );
    net = cropTempl(net);
    GaussianBlur(net, net, dsize, 5.0);
    
    // Trident 
    cvtColor( trident, trident, CV_BGR2HSV ); //convert to hsv colors
    inRange( trident, Scalar((hueLow/360.0)*255, 
                         (satLow/100.0)*255, 
                         (valueLow/100.0)*255, 0),
                 Scalar((hueHigh/360.0)*255,
                         (satHigh/100.0)*255, 
                         (valueHigh/100.0)*255, 0),
                 trident );
    trident = cropTempl(trident);
    GaussianBlur(trident, trident, dsize, 5.0);*/
    
    // MAIN LOOP
    //while(1)
    //{  
		// Capture image from camera
		//cap >> query;

		// Set colors vector
        vector<Scalar> colors;
        colors.push_back(Scalar(0,0,255));
        colors.push_back(Scalar(0,255,0));
        colors.push_back(Scalar(230,0,230));
        colors.push_back(Scalar(200,150,50));
        
        // Convert to HSV    
        Mat hsv;
        Mat mask;
        cvtColor( query, hsv, CV_BGR2HSV ); //convert to hsv colors
        inRange( hsv, Scalar((hueLow/360.0)*255, 
                             (satLow/100.0)*255, 
                             (valueLow/100.0)*255, 0),
                     Scalar((hueHigh/360.0)*255,
                             (satHigh/100.0)*255, 
                             (valueHigh/100.0)*255, 0),
                     mask );

        // Vector containing rectangles of all red shapes
        vector<Rect> rois = findShapes(mask);
        
        //  cout << "Rois.size(): " << rois.size() << endl;
        
        // for user, not requred for calculation
        imshow("mask", mask);
        
        // Run match on each roi in rois 
        for ( size_t i = 0; i < rois.size(); i++ )
        {
            int deg = 0; // degrees of rotation of template image
            Mat roi = mask(rois[i]); // Go through each roi

            // ROTATION OF TEMPLATE IMAGE:
            //for(deg = 0; deg < 360; deg += 15)
            //{
                //GaussianBlur(roi, roi, dsize, 5.0);

            
                //if ( shieldSize( roi, shield, 3 ) )
                if(strcmp(imName, "shield") == 0){
                  if ( MatchingMethod( roi, shield, 3 ) )
                  {
                      putText( query, "Shield", cvPoint(rois[i].x + 10, rois[i].y + 30),  
                               1, 1.0, colors[0]);
                    /*cout << "Shield " 
                         << "Position " << cvPoint(rois[i].x + 10, rois[i].y + 30) 
                         << "Width: " << roi.cols 
                         << "Height: " << roi.rows
                         << "Rotation: " << deg << endl;*/
                      rectangle( query, rois[i], colors[0]);
                      return 1;
                  }
                }
                roi = mask(rois[i]);
               
                //roi = mask(rois[i]);
                /*if ( Match( net, roi ) )
                {
                    putText( query, "Net", cvPoint(rois[i].x + 10, rois[i].y + 30),  
                             1, 1.0, colors[0]);
                    cout << "Net " 
                         << "Position " << cvPoint(rois[i].x + 10, rois[i].y + 30) 
                         << "Width: " << roi.cols 
                         << "Height: " << roi.rows
                         << "Rotation: " << deg << endl;
                    rectangle( query, rois[i], colors[0]);
                }
                */
                /*if ( Match( trident, roi ) )
                {
                    putText( query, "Trident", cvPoint(rois[i].x + 10, rois[i].y + 30),  
                             1, 1.0, colors[0]);
                    // Format:Name Position Scale Rotation
                    cout << "Trident " << "Position " << cvPoint(rois[i].x + 10, rois[i].y + 30) 
                         << "Width: " << roi.cols 
                         << "Height: " << roi.rows
                         << "Rotation: " << deg << endl;
                    rectangle( query, rois[i], colors[0]);
                }*/
               
               // roi = mask(rois[i]);
                if(strcmp(imName, "sword") == 0){
                  if ( Match( sword, roi ) )
                  {
                      putText( query, "Sword", cvPoint(rois[i].x + 10, rois[i].y + 30),  
                             1, 1.0, colors[0]);
                    // Format:Name Position Scale Rotation
                    /*cout << "Sword " 
                         << "Position " << cvPoint(rois[i].x + 10, rois[i].y + 30) 
                         << "Width: " << roi.cols 
                         << "Height: " << roi.rows
                         << "Rotation: " << deg << endl;*/
                      rectangle( query, rois[i], colors[0]);
                      return 1;
                  }
                }
            //}
        }

        // for user, not requred for calculation
        imshow("Image", query );

        // colorMatch( );
        if(waitKey(1) >= 0) break;
    //}
}



bool Match( Mat& query, Mat& roi )
{
    namedWindow( "Q", CV_WINDOW_AUTOSIZE );
    namedWindow( "R", CV_WINDOW_AUTOSIZE );
    namedWindow( "Q1", CV_WINDOW_AUTOSIZE );
    namedWindow( "R1", CV_WINDOW_AUTOSIZE );
    
    Size    dsize(9.0,9.0);
    GaussianBlur(roi, roi, dsize, 5.0);
    
    // for user, not requred for calculation
    imshow("Q1", query);
    imshow("R1", roi);
    
    Mat new_query;
    Mat new_roi;
    Mat result;
    Mat new_result;
    
  //  query.copyTo(result);
    
    query.copyTo(new_query);
    roi.copyTo(new_roi);
    
    
    // For future look into this for scaling: 
        //http://docs.opencv.org/doc/tutorials/imgproc/pyramids/pyramids.html#pyramids
    // Scale both to same size
    if( query.size().width > roi.size().width )
   {
        //scale query to roi
        resize(query, new_query, roi.size());
    }
    else
    {
        //Scale roi to query
        resize(roi, new_roi, query.size());
    }
   
    // for user, not requred for calculation
    imshow("Q", new_query);
    imshow("R", new_roi);
    
    /// Simple subtraction
    
    //subtract( new_query, new_roi, result, noArray(), 2);
    //compare(new_query, new_roi, result, CMP_EQ);
    absdiff(new_query, new_roi, result);  // Sword and Trident
    
     
    Scalar average = mean(result);
    
    
//    double determinant(InputArray mtx)
   
  //  bool eigen(InputArray src, OutputArray eigenvalues, int lowindex=-1, int highindex=-1)
   
    //double Mahalanobis(new_query, new_roi, InputArray icovar)
   
    //cout << "average: " << norm(new_query, new_roi, NORM_INF) << endl;
    
    if((double)average[0] < 50.0 )
        return true;
    else
        return false;
}

bool shieldSize( Mat frame, Mat templ, int name )
{
    Size frameSize = frame.size();
    double frameArea = frameSize.width * frameSize.height;
    return ( currentContArea / frameArea ) > SHIELD_THRESHOLD;
}

/* Matching Function */
bool MatchingMethod( Mat frame, Mat templ, int name )
{

    /* Create windows */
    // namedWindow( "frame", CV_WINDOW_AUTOSIZE );
    Mat new_frame;
    
    resize(frame, new_frame, Size(templ.size().width/2, templ.size().height/2));
    
    Size    dsize(3.0,3.0);
    
    Mat result;

    new_frame.copyTo( result );
    /* Create the result matrix */

    result.create( result.cols, result.rows, CV_32FC1 );
    
    GaussianBlur(new_frame, new_frame, dsize, 1.0);
        
    //imshow ( "frame", frame );
    matchTemplate( new_frame, templ, result, CV_TM_SQDIFF_NORMED);

    // brings all of the data in the result image into a useable range
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        
    /* Localizing the best match with minMaxLoc */
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    
    /* Determines the location of the highest and lowest values of the 
        result array to determine the areas with the best or worst matches. */
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    /* Rename for easier naming convention */
    /* If using SQDIFF or SQDIFF_NORMED then want minLoc, otherwise maxLoc */
    matchLoc = minLoc;

    // if 0 or above, not a good enough match // helps avoid false positives
    if ( minVal < 0 ) // look for a match
        return true;
    else
        return false;
}

vector<Rect> findShapes(Mat& mask)
{
    Mat edgeDetect;
    Canny(mask, edgeDetect, 50, 200, 3);

    vector<vector<Point> > contours;
    
    Mat maskCpy;
    mask.copyTo(maskCpy);
    findContours(maskCpy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    // Define array to hold bounding rectangles for red areas
    vector<Rect> ret;
    
    Size frame_size = mask.size();
        double frame_area = frame_size.width * frame_size.width +
                            frame_size.height * frame_size.height;
    
    for ( size_t i = 0; i < contours.size(); i++ )
    {
        currentContArea = fabs(contourArea(Mat(contours[i])));
        if ( currentContArea > FILTER_PERCENT * frame_area ) // Make sure it's a big red piece
        {
            ret.push_back( boundingRect(Mat(contours[i])) );
        }
    }
    return ret;
}

Mat cropTempl(Mat& templ )
{
    Mat edgeDetect;
    Canny(templ, edgeDetect, 50, 200, 3);

    vector<vector<Point> > contours;
    
    Mat templCpy;
    templ.copyTo(templCpy);
    findContours(templCpy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    Mat newTempl = templ(boundingRect(Mat(contours[0])) );

    return templ;
}
