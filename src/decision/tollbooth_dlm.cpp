// Copyright 2013 Robosub Club of the Palouse

#include "decision/tollbooth_dlm.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>
#include "utility/DebugLog.hpp"


using ::cv::imread;

namespace decision {
  // TODO(LPE) Implement.
  bool TollboothDLM::can_assume_control() {
    DEBUG_METHOD();
    return false;
  }


  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_tollbooth_dlm() {
    DEBUG_METHOD();
    return string("can_call_tollbooth_dlm");
  }
}  // namespace decision

