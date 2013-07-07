// Copyright 2013 Robosub Club of the Palouse

#include "decision/tollbooth_dlm.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>
#include "utility/DebugLog.hpp"

using ::cv::imread;

namespace decision {
  TollboothDLM::TollboothDLM(movement::FuzzySets *fuzzy_sub_state,
                             cv::VideoCapture forward_vidcap,
                             cv::VideoCapture downward_vidcap)
    : DecisionLogicModule(fuzzy_sub_state, forward_vidcap, downward_vidcap) {
    DEBUG_METHOD();
    m_forward_eye = new vision::TollboothEye(m_forward_vidcap);
    m_downward_eye = new vision::TollboothEye(m_downward_vidcap);
  }

  // TODO(LPE) Implement.
  bool TollboothDLM::can_assume_control() {
    return true;
  }

  // TODO(LPE) Implement.
  void TollboothDLM::activate() {
    DEBUG_METHOD();
    set_mission_accomplished(true);
    return;
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_tollbooth_dlm() {
    DEBUG_METHOD();
    return string("can_call_tollbooth_dlm");
  }
}  // namespace decision

