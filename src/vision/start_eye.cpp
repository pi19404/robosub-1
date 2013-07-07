// Copyright 2013 Robosub Club of the Palouse

#include "vision/start_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"
#include "vision/base_eye.h"

using ::cv::imread;

namespace vision {
  StartEye::StartEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
    DEBUG_METHOD();
  }

  StartEye::StartEye(std::string filename)
  : BaseEye() {
    DEBUG_METHOD();
    m_frame = imread(filename);
  }

  StartEye::~StartEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool StartEye::can_see_target() {
    return false;
  }
}  // namespace vision

