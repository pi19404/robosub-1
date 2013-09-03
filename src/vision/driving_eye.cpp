// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#include "vision/driving_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"
#include "vision/base_eye.h"

using ::cv::imread;

namespace vision {
  DrivingEye::DrivingEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
    DEBUG_METHOD();
  }

  DrivingEye::DrivingEye(std::string filename)
  : BaseEye() {
    DEBUG_METHOD();
    m_frame = imread(filename);
  }

  DrivingEye::~DrivingEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool DrivingEye::can_see_target() {
    return false;
  }
}  // namespace vision

