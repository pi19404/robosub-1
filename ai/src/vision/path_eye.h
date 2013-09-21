// Copyright 2013 Robosub Club of the Palouse

#ifndef SRC_VISION_PATH_EYE_H_
#define SRC_VISION_PATH_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class PathEye : public BaseEye {
   public:
    explicit PathEye(cv::VideoCapture vidcap);
    // For testing.
    explicit PathEye(std::string filename);
    virtual ~PathEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_PATH_EYE_H_

