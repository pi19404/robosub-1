// COPYRIGHT: Robosub Club of the Palouse under the GPL v3

#ifndef SRC_DECISION_MOCK_DLM_H_
#define SRC_DECISION_MOCK_DLM_H_

#include <cv.h>
#include <highgui.h>
#include <string>

#include "decision/base_dlm.h"
#include "vision/base_eye.h"
#include "movement/fuzzy_sets.h"

using ::std::string;

namespace decision {
  class MockDLM : public DecisionLogicModule {
   protected:
    vision::BaseEye *m_forward_eye;
    vision::BaseEye *m_downward_eye;

   public:
    MockDLM(movement::FuzzySets *fuzzy_sub_state,
            cv::VideoCapture forward_vidcap,
            cv::VideoCapture downward_vidcap)
    : DecisionLogicModule(fuzzy_sub_state, forward_vidcap, downward_vidcap) {
      m_forward_eye = new vision::BaseEye(m_forward_vidcap);
      m_downward_eye = new vision::BaseEye(m_downward_vidcap);
    }

    // TODO(LPE) Implement.
    virtual void activate();
    virtual bool can_assume_control();
    bool m_can_assume_control_mock_ret;
    // This is here to change the protection level from protected.
    virtual void set_mission_accomplished(bool setting);
  };

  string can_call_tollbooth_dlm();
}  // namespace decision

#endif  // SRC_DECISION_MOCK_DLM_H_

