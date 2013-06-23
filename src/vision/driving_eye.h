// Copyright 2013 Robosub Club of the Palouse

#ifndef SRC_VISION_DRIVING_EYE_H_
#define SRC_VISION_DRIVING_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class DrivingEye : public BaseEye {
   public:
    explicit DrivingEye(cv::VideoCapture vidcap);
    // For testing.
    explicit DrivingEye(std::string filename);
    virtual ~DrivingEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_DRIVING_EYE_H_

