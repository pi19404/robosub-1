#include <string>

#include "vision_utilities.h"

using ::std::string;

#define PI 3.1415926

namespace vision {
  // Returns the angle in degrees.
  double angle(Point corner, Point endA, Point endB) {
    double sideA2, sideB2, side_opposite2, inter_result;
    // sideA2 is the squared length oposite the point endA. There is
    // no point in taking the square root, because the law of
    // cosines would have us square the side lengths again.
    sideA2 = (corner.x - endB.x) * (corner.x - endB.x) +
             (corner.y - endB.y) * (corner.y - endB.y);
    sideB2 = (corner.x - endA.x) * (corner.x - endA.x) +
             (corner.y - endA.y) * (corner.y - endA.y);
    side_opposite2 = sqrt((endA.x - endB.x) * (endA.x - endB.x) +
                          (endA.y - endB.y) * (endA.y - endB.y));
    inter_result = (sideA2 + sideB2 - side_opposite2) /
                   (2.0 * sqrt(sideA2) * sqrt(sideB2));
    return acos(inter_result) * 180.0 / PI;
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_vision_utilities() {
    return string("can_call_vision_utilities");
  }
}  // namespace vision

