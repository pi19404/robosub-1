// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#ifndef SRC_VISION_PARKING_EYE_H_
#define SRC_VISION_PARKING_EYE_H_

#include "vision/parking_eye.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include "vision/base_eye.h"

namespace vision {
  class ParkingEye : public BaseEye {
   public:
    explicit ParkingEye(cv::VideoCapture vidcap);
    // For testing.
    explicit ParkingEye(std::string filename);
    virtual ~ParkingEye();

    virtual bool can_see_target();
  };
}  // namespace vision

#endif  // SRC_VISION_PARKING_EYE_H_

