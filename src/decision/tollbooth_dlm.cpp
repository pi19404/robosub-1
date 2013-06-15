#include "./tollbooth_dlm.h"

#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>

using ::cv::imread;

namespace decision {
  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_tollbooth_dlm() {
    return string("can_call_tollbooth_dlm");
  }
}  // namespace decision

