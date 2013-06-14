#ifndef SRC_VISION_VISION_UTILITIES_H_
#define SRC_VISION_VISION_UTILITIES_H_

#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <string>

using ::cv::Point;
using ::std::string;

namespace vision {
  double angle(Point corner, Point end1, Point end2);
  string can_call_vision_utilities();
}  // namespace vision

#endif  // SRC_VISION_VISION_UTILITIES_H_

