// Copyright 2013 Robosub Club of the Palouse

#include "vision/tollbooth_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>
#include "vision/vision_utilities.h"

using ::std::vector;
using ::std::string;
using ::cv::imread;
using ::cv::Mat;
using ::cv::Rect;
using ::cv::Point;

namespace vision {
  TollboothEye::TollboothEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
  }

  TollboothEye::TollboothEye(string filename)
  : BaseEye() {
    m_frame = imread(filename);
  }

  TollboothEye::~TollboothEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool TollboothEye::can_see_target() {
    return false;
  }

  // TODO(LPE): Implement
  vector<vector<Point> >* TollboothEye::get_tollbooth_bounding_box(
      Mat frame) {
    using ::cv::Point;
    double x = angle(Point(0, 0), Point(0, 0), Point(0, 0));
    return NULL;
  }

  // TODO(LPE): Implement
  Rect TollboothEye::get_tollbooth_flattened_rect(
      Mat frame, vector<vector<Point> > box) {
    cv::Rect ret;
    return ret;
  }

  // TODO(LPE): Implement
  vector<Rect>* TollboothEye::get_tollbooth_windows(Mat frame, Rect rect) {
    return NULL;
  }

  // TODO(LPE): Implement
  color_t TollboothEye::get_tollbooth_window_color(
      Mat frame, Rect tollbooth_window) {
    return red;
  }

  // TODO(LPE): Implement
  Point TollboothEye::get_tollbooth_window_large_hex_center(
      Mat frame, Rect toolbooth_window) {
    cv::Point ret;
    return ret;
  }

  // TODO(LPE): Implement
  Point TollboothEye::get_tollbooth_window_small_hex_center(
      Mat frame, Rect toolbooth_window) {
    cv::Point ret;
    return ret;
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_tollbooth_eye() {
    return string("can_call_tollbooth_eye");
  }
}  // namespace vision

