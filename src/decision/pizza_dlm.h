// COPYRIGHT: Robosub Club of the Palouse under the GPL v3

#ifndef SRC_DECISION_PIZZA_DLM_H_
#define SRC_DECISION_PIZZA_DLM_H_

#include <cv.h>
#include <highgui.h>
#include <string>

#include "decision/base_dlm.h"
#include "vision/pizza_eye.h"
#include "movement/fuzzy_sets.h"

using ::std::string;

namespace decision {
  class PizzaDLM : public DecisionLogicModule {
   protected:
    vision::PizzaEye *m_forward_eye;
    vision::PizzaEye *m_downward_eye;

   public:
    PizzaDLM(movement::FuzzySets *fuzzy_sub_state,
             cv::VideoCapture forward_vidcap,
             cv::VideoCapture downward_vidcap);
    virtual bool can_assume_control();
    virtual void activate();
  };

  string can_call_pizza_dlm();
}  // namespace decision

#endif  // SRC_DECISION_PIZZA_DLM_H_

