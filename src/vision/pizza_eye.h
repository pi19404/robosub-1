// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#ifndef SRC_VISION_PIZZA_EYE_H_
#define SRC_VISION_PIZZA_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class PizzaEye : public BaseEye {
   public:
    explicit PizzaEye(cv::VideoCapture vidcap);
    // For testing.
    explicit PizzaEye(std::string filename);
    virtual ~PizzaEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_PIZZA_EYE_H_

