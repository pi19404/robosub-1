#include "./detect_hexagon.h"

namespace vision {
  TollboothDetector::TollboothDetector(string filename) {
    m_frame = imread(filename);
  }

  TollboothDetector::TollboothDetector(Mat frame) {
    m_frame = frame;
  }

  TollboothDetector::~TollboothDetector() {
    m_frame.release();
  }

  // TODO: Implement
  bool TollboothDetector::can_see_tollbooth() {
    return false;
  }

  // TODO: Implement
  vector<vector<Point> >* TollboothDetector::get_tollbooth_bounding_box(
      Mat frame) {
    return NULL;
  }

  // TODO: Implement
  Rect TollboothDetector::get_tollbooth_flattened_rect(
      Mat frame, vector<vector<Point> > box) {
    cv::Rect ret;
    return ret;
  }

  // TODO: Implement
  vector<Rect>* TollboothDetector::get_tollbooth_windows(Mat frame, Rect rect) {
    return NULL;
  }

  // TODO: Implement
  color_t TollboothDetector::get_tollbooth_window_color(
      Mat frame, Rect tollbooth_window) {
    return red;
  }

  // TODO: Implement
  Point TollboothDetector::get_tollbooth_window_large_hex_center(
      Mat frame, Rect toolbooth_window) {
    cv::Point ret;
    return ret;
  }

  // TODO: Implement
  Point TollboothDetector::get_tollbooth_window_small_hex_center(
      Mat frame, Rect toolbooth_window) {
    cv::Point ret;
    return ret;
  }
}

// This is a throwaway stub used to
// confirm that gtest works.
int testing_gtest() {
  return 1337;
}

