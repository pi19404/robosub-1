// Copyright 2013 Robosub Club of the Palouse

#include "vision/base_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"

using ::std::string;

namespace vision {
  BaseEye::BaseEye() {
    DEBUG_METHOD();
  }

  BaseEye::~BaseEye() {
  }

  BaseEye::BaseEye(cv::VideoCapture vidcap) {
    m_vidcap = vidcap;
  }

  bool BaseEye::can_see_target() {
    return false;
  }

  void BaseEye::blink() {
    m_vidcap >> m_frame;
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_base_eye() {
    return string("can_call_base_eye");
  }
}  // namespace vision

