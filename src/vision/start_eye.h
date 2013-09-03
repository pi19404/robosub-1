// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#ifndef SRC_VISION_START_EYE_H_
#define SRC_VISION_START_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class StartEye : public BaseEye {
   public:
    explicit StartEye(cv::VideoCapture vidcap);
    // For testing.
    explicit StartEye(std::string filename);
    virtual ~StartEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_START_EYE_H_

