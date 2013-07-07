// Copyright 2013 Robosub Club of the Palouse

#ifndef SRC_STATE_STATE_MACHINE_H_
#define SRC_STATE_STATE_MACHINE_H_

#include <cv.h>
#include <highgui.h>
#include <vector>
#include <map>
#include <string>
#include "decision/base_dlm.h"
#include "movement/fuzzy_sets.h"

using ::std::vector;
using ::std::map;
using ::std::string;

namespace state {
  // NUM_TASKS must be listed last.
  enum task_t {
    START, PATH, TRAFFIC_LIGHT, PARKING, SPEED_TRAP, TOLLBOOTH, DRIVING, PIZZA,
    NUM_TASKS
  };

  class StateMachine {
   private:
    movement::FuzzySets *m_fuzzy_sub_state;
    bool m_can_transition[NUM_TASKS][NUM_TASKS];
    // This vector is a convenience for iteration.
    map<task_t, decision::DecisionLogicModule*> m_task_to_dlm;
   public:
    // TODO(LPE) The state machine should also accept an object that defines
    // communication protocols with the Arduino. These will be needed to fully
    // initialize all of the movement modules.
    StateMachine(cv::VideoCapture forward_eye, cv::VideoCapture downward_eye);
    ~StateMachine();
    void enter();
  };
}  // namespace state

#endif  // SRC_STATE_STATE_MACHINE_H_

