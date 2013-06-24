// Copyright 2013 Robosub Club of the Palouse

#ifndef SRC_VISION_SPEED_TRAP_EYE_H_
#define SRC_VISION_SPEED_TRAP_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class SpeedTrapEye : public BaseEye {
   public:
    explicit SpeedTrapEye(cv::VideoCapture vidcap);
    // For testing.
    explicit SpeedTrapEye(std::string filename);
    virtual ~SpeedTrapEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_SPEED_TRAP_EYE_H_

