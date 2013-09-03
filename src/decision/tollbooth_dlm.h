// COPYRIGHT: Robosub Club of the Palouse under the GPL v3

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
                 cv::VideoCapture downward_vidcap);
    virtual bool can_assume_control();
    virtual void activate();
  };

  string can_call_tollbooth_dlm();
}  // namespace decision

#endif  // SRC_DECISION_TOLLBOOTH_DLM_H_

