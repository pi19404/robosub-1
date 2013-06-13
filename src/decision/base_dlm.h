#ifndef SRC_DECISION_DLM_BASE_H_
#define SRC_DECISION_DLM_BASE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include "fuzzy_sets.h"

using ::std::string;

namespace decision {
  class DecisionLogicModule {
   protected:
    movement::FuzzySets *m_fuzzy_sub_state;
    cv::VideoCapture m_forward_vidcap;
    cv::VideoCapture m_downward_vidcap;
   public:
    DecisionLogicModule(movement::FuzzySets *fuzzy_sub_state,
                        cv::VideoCapture forward_vidcap,
                        cv::VideoCapture downward_vidcap) {
      m_fuzzy_sub_state = fuzzy_sub_state;
      m_forward_vidcap = forward_vidcap;
      m_downward_vidcap = downward_vidcap;
    }
  };

  string can_call_base_dlm();
}  // namespace decision

#endif  // SRC_DECISION_DLM_BASE_H_

