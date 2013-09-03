// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#ifndef SRC_VISION_TRAFFIC_LIGHT_EYE_H_
#define SRC_VISION_TRAFFIC_LIGHT_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class TrafficLightEye : public BaseEye {
   public:
    explicit TrafficLightEye(cv::VideoCapture vidcap);
    // For testing.
    explicit TrafficLightEye(std::string filename);
    virtual ~TrafficLightEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_TRAFFIC_LIGHT_EYE_H_

