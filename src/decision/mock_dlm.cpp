// Copyright 2013 Robosub Club of the Palouse

// This file only exists to simplify some of the testing. It should
// NOT be used in production. If the MockDLM object is used anywhere
// in ai/src and it doesn't have a big red TODO(information contact initials),
// then someone goofed up!

#include "decision/mock_dlm.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>


using ::cv::imread;

namespace decision {
  void MockDLM::activate() {
    m_can_assume_control_mock_ret = false;
  }

  bool MockDLM::can_assume_control() {
    return m_can_assume_control_mock_ret;
  }

  void MockDLM::set_mission_accomplished(bool setting) {
    DecisionLogicModule::set_mission_accomplished(setting);
  }
}  // namespace decision

