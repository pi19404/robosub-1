// Copyright 2013 Robosub Club of the Palouse

#ifndef SRC_DECISION_TOLLBOOTH_DLM_H_
#define SRC_DECISION_TOLLBOOTH_DLM_H_

#include <cv.h>
#include <highgui.h>
#include <string>

#include "decision/base_dlm.h"
#include "vision/tollbooth_eye.h"
#include "movement/fuzzy_sets.h"

using ::std::string;

namespace decision {
  class TollboothDLM : public DecisionLogicModule {
   protected:
    vision::TollboothEye *m_forward_eye;
    vision::TollboothEye *m_downward_eye;

   public:
    TollboothDLM(movement::FuzzySets *fuzzy_sub_state,
                 cv::VideoCapture forward_vidcap,
                 cv::VideoCapture downward_vidcap)
    : DecisionLogicModule(fuzzy_sub_state, forward_vidcap, downward_vidcap) {
      m_forward_eye = new vision::TollboothEye(m_forward_vidcap);
      m_downward_eye = new vision::TollboothEye(m_downward_vidcap);
    }

    // TODO(LPE) Implement.
    virtual void activate() {
      return;
    }
  };

  string can_call_tollbooth_dlm();
}  // namespace decision

#endif  // SRC_DECISION_TOLLBOOTH_DLM_H_

