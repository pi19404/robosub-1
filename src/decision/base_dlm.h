#ifndef SRC_DECISION_DLM_BASE_H_
#define SRC_DECISION_DLM_BASE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
using ::std::string;

namespace decision {
  class DecisionLogicModule {
   protected:
    cv::VideoCapture m_forward_video_cap;
    cv::VideoCapture m_downward_video_cap;
    // FuzzySetMembership *m_membership;
   public:
    DecisionLogicModule(cv::VideoCapture forward_video_cap,
                        cv::VideoCapture downward_video_cap)
    : m_forward_video_cap(forward_video_cap),
      m_downward_video_cap(downward_video_cap) {
    }

    // If there is a movement_logic_module,
    // then there will be an object that holds all of the fuzzy membership
    // values. That object should be protected with a mutex, because all of
    // the DLMs will need to write to it at some point. The state machine
    // should keep everything from conflicting, but the added protection of
    // a mutix wouldn't hurt. So, the DLM will need to have a method that
    // specifies the location of the Fuzzy Set Membership object. This
    // code will look something like this:
    //
    // void FuzzySetMemberShip::set_sub_is_rotated_left(double membership) {
    //   assert(0.0 <= membership && membership <= 1.0);
    //   boost::lock_guard<boost::mutex> (access_rotated_left_mut());
    //   m_sub_is_rotated_left = membership;
    // }
  };

  string can_call_base_dlm();
}  // namespace decision

#endif  // SRC_DECISION_DLM_BASE_H_

