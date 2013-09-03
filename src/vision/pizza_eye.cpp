// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#include "vision/pizza_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "utility/DebugLog.hpp"
#include "vision/base_eye.h"

using ::cv::imread;

namespace vision {
  PizzaEye::PizzaEye(cv::VideoCapture vidcap)
  : BaseEye(vidcap) {
    DEBUG_METHOD();
  }

  PizzaEye::PizzaEye(std::string filename)
  : BaseEye() {
    DEBUG_METHOD();
    m_frame = imread(filename);
  }

  PizzaEye::~PizzaEye() {
    m_frame.release();
  }

  // TODO(LPE): Implement
  bool PizzaEye::can_see_target() {
    return false;
  }
}  // namespace vision

