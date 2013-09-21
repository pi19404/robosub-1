// Copyright 2013 Robosub Club of the Palouse

#include "vision/path_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"
#include "vision/base_eye.h"

using ::cv::imread;

namespace vision {
  PathEye::PathEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
    DEBUG_METHOD();
  }

  PathEye::PathEye(std::string filename)
  : BaseEye() {
    DEBUG_METHOD();
    m_frame = imread(filename);
  }

  PathEye::~PathEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool PathEye::can_see_target() {
    return false;
  }
}  // namespace vision

