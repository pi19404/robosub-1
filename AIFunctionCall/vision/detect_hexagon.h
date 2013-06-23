#ifndef AI_AIFUNCTIONCALL_VISION_H_
#define AI_AIFUNCTIONCALL_VISION_H_

#include <vector>
#include <cv.h>
#include <highgui.h>

using ::std::vector;

namespace vision {
using namespace cv;
  enum color_t {green, blue, pink, red};

  class TollboothDetector {
   protected:
    Mat m_frame;

   public:
    TollboothDetector(string filename);
    TollboothDetector(Mat frame);
    ~TollboothDetector();

    bool can_see_tollbooth();
    vector<vector<Point> >* get_tollbooth_bounding_box(Mat frame);
    Rect get_tollbooth_flattened_rect(Mat frame, vector<vector<Point> > box);
    vector<Rect>* get_tollbooth_windows(Mat frame, Rect rect);
    color_t get_tollbooth_window_color(Mat frame, Rect tollbooth_window);
    Point get_tollbooth_window_large_hex_center(Mat frame,
                                                Rect toolbooth_window);
    Point get_tollbooth_window_small_hex_center(Mat frame,
                                                Rect toolbooth_window);
  };
}

int testing_gtest();

#endif  // AI_AIFUNCTIONCALL_VISION_H_

