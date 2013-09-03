// COPYRIGHT: Robosub Club of the Palouse under the GPL v3

#ifndef SRC_DECISION_BASE_DLM_H_
#define SRC_DECISION_BASE_DLM_H_

#include <cv.h>
#include <highgui.h>
#include <string>

#include "movement/fuzzy_sets.h"

using ::std::string;


namespace decision {
  enum {MIN_CYCLE_MICROSEC = 1000000};

  class DecisionLogicModule {
   protected:
    movement::FuzzySets *m_fuzzy_sub_state;
    cv::VideoCapture m_forward_vidcap;
    cv::VideoCapture m_downward_vidcap;
    bool m_mission_accomplished;
    virtual void set_mission_accomplished(bool setting);
   public:
    DecisionLogicModule(movement::FuzzySets *fuzzy_sub_state,
                        cv::VideoCapture forward_vidcap,
                        cv::VideoCapture downward_vidcap) {
      m_fuzzy_sub_state = fuzzy_sub_state;
      m_forward_vidcap = forward_vidcap;
      m_downward_vidcap = downward_vidcap;
      m_mission_accomplished = false;
    }

    virtual bool get_mission_accomplished();

    virtual void activate()=0;
    virtual bool can_assume_control()=0;
  };

  string can_call_base_dlm();
}  // namespace decision

#endif  // SRC_DECISION_BASE_DLM_H_

