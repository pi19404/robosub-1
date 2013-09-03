// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#include "vision/speed_trap_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"
#include "vision/base_eye.h"

using ::cv::imread;

namespace vision {
  SpeedTrapEye::SpeedTrapEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
    DEBUG_METHOD();
  }

  SpeedTrapEye::SpeedTrapEye(std::string filename)
  : BaseEye() {
    DEBUG_METHOD();
    m_frame = imread(filename);
  }

  SpeedTrapEye::~SpeedTrapEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool SpeedTrapEye::can_see_target() {
    return false;
  }
}  // namespace vision

