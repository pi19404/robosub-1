// Copyright 2013 Robosub Club of the Palouse

#include "vision/traffic_light_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"
#include "vision/base_eye.h"

using ::cv::imread;

namespace vision {
  TrafficLightEye::TrafficLightEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
    DEBUG_METHOD();
  }

  TrafficLightEye::TrafficLightEye(std::string filename)
  : BaseEye() {
    DEBUG_METHOD();
    m_frame = imread(filename);
  }

  TrafficLightEye::~TrafficLightEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool TrafficLightEye::can_see_target() {
    return false;
  }
}  // namespace vision

