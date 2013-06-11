/* Kenneth Perrault
 * This file contains the function checkPath().
 * checkPath() is used to identify whether or not orange path markers
 * are visible to the downward facing camera.
 *
 * Parameters:
 * - double *angleDegrees
 *
 * Return value:
 * Bool
 *
 * Description:
 * Axis - visualize a line going from the bottom of the image to
 *        top of the image, and passing through the center of the image.
 *
 * If one path marker is visible to the downward facing camera, its
 * angle relative to the image axis is returned. Positive values indicate
 * the path is to the right of the axis, negative is to the left. 
 *
 * IF THERE ARE TWO PATHS, THIS FUNCTION CHOOSES TO ONLY FOCUS ON
 * THE RIGHT PATH. FOR THE SPRING 2013 TEAM, WE DID NOT ATTEMPT THE
 * OBSTACLE AT THE END OF THE LEFT PATH. SO WE ALWAYS GO RIGHT.
 *
 * If no paths are seen, false is returned. Otherwise true is returned.
 *
*/

int hueLow = 10;
int hueHigh = 25;
int satLow = 43;
int satHigh = 100;
int valueLow = 54;
int valueHigh = 100;

bool checkPath(double *angleDegrees)
{
  /* Get the first camera */
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  bool pathSeen = false;

  Mat frame;
  Mat image;
  Mat hsv;
  Mat mask;
  Mat lineDetect;
  Mat nullMat;
  Mat crop;
  Point nullPoint(-1,-1);

  // Get an image from the camera
  cap >> frame;
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

  // OpenCV-2.3.1/samples/cpp/houghlines.cpp crashed (after copy pasta)
  // for some reason.

  // I used the example in the docs for HoughLinesP.

  if(rectangles.size() == 0)
    pathSeen = false;
  else
  {
    pathSeen = true;
    Rect path = rectangles.front();
    if(rectangles.size() > 1)
    {
      rectangles.pop_front();
      Rect tmp = rectangles.front();
      if(tmp.x > path.x) // always choose the right path
        path = tmp;

    }

    // crop out the right path from the image
    crop = mask(path);

    // find all orange lines in the cropped out path
    Canny(crop, lineDetect, 50, 200, 3);
    vector<Vec4i> lines;
    HoughLinesP( lineDetect, lines,
               1,         // rho
               CV_PI/180, // theta
               40,        // threshhold
               10,        // min-length
               10         // max-line-gap
              );


    // put the lines on the cropped image
    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( crop, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }


    //http://stackoverflow.com/questions/491738/how-do-you-calculate-the-average-of-a-set-of-angles
    // calculate the average angle of all the lines in the cropped image
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
    *angleDegrees = (angle * 180) / CV_PI;

    // adjust the value to be the degrees in which the robosub should turn.
    // negative degrees indicate turning left
    // positive degrees indicate turning right
    if(*angleDegrees < 0)
      *angleDegrees = 90 + *angleDegrees;
    else
      *angleDegrees = -1 * (90 - *angleDegrees);
  }

  return pathSeen;
}
