#ifndef SRC_VISION_TOLLBOOTH_EYE_H_
#define SRC_VISION_TOLLBOOTH_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <vector>

#include "vision_utilities.h"

using ::std::vector;
using ::std::string;
using ::cv::Mat;
using ::cv::Rect;
using ::cv::Point;

namespace vision {
  enum color_t {green, blue, pink, red};

  class TollboothEye {
   protected:
    Mat m_frame;

   public:
    TollboothEye();
    // For testing.
    TollboothEye(string filename);
    ~TollboothEye();

    bool can_see_tollbooth();
    void update_frame(cv::VideoCapture& vidcap);
    vector<vector<Point> >* get_tollbooth_bounding_box(Mat frame);
    Rect get_tollbooth_flattened_rect(Mat frame, vector<vector<Point> > box);
    vector<Rect>* get_tollbooth_windows(Mat frame, Rect rect);
    color_t get_tollbooth_window_color(Mat frame, Rect tollbooth_window);
    Point get_tollbooth_window_large_hex_center(Mat frame,
                                                Rect toolbooth_window);
    Point get_tollbooth_window_small_hex_center(Mat frame,
                                                Rect toolbooth_window);
  };

  string can_call_tollbooth_eye();
}

#endif  // SRC_VISION_TOLLBOOTH_EYE_H_

