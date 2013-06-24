// Copyright 2013 Robosub Club of the Palouse

#ifndef SRC_DECISION_PATH_DLM_H_
#define SRC_DECISION_PATH_DLM_H_

#include <cv.h>
#include <highgui.h>
#include <string>

#include "decision/base_dlm.h"
#include "vision/path_eye.h"
#include "movement/fuzzy_sets.h"

using ::std::string;

namespace decision {
  class PathDLM : public DecisionLogicModule {
   protected:
    vision::PathEye *m_forward_eye;
    vision::PathEye *m_downward_eye;

   public:
    PathDLM(movement::FuzzySets *fuzzy_sub_state,
            cv::VideoCapture forward_vidcap,
            cv::VideoCapture downward_vidcap);
    virtual bool can_assume_control();
    virtual void activate();
  };

  string can_call_path_dlm();
}  // namespace decision

#endif  // SRC_DECISION_PATH_DLM_H_

