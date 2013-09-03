// COPYRIGHT: Robosub Club of the Palouse under the GPL v3

#include "decision/tollbooth_dlm.h"
#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "utility/DebugLog.hpp"

using ::cv::imread;

namespace decision {
  TollboothDLM::TollboothDLM(movement::FuzzySets *fuzzy_sub_state,
                             cv::VideoCapture forward_vidcap,
                             cv::VideoCapture downward_vidcap)
    : DecisionLogicModule(fuzzy_sub_state, forward_vidcap, downward_vidcap) {
    DEBUG_METHOD();
    m_forward_eye = new vision::TollboothEye(m_forward_vidcap);
    m_downward_eye = new vision::TollboothEye(m_downward_vidcap);
  }

  // TODO(LPE) Implement.
  bool TollboothDLM::can_assume_control() {
    return true;
  }

  // TODO(LPE) Implement.
  void TollboothDLM::activate() {
    DEBUG_METHOD();
    boost::posix_time::ptime compute_start, compute_done, cycle_done;
    boost::posix_time::time_duration cycle_taken, cycle_remaining, cycle_total;

    while (!this->get_mission_accomplished()) {
      compute_start = boost::posix_time::microsec_clock::local_time();
      // Do stuff
      boost::this_thread::sleep(boost::posix_time::microseconds(10000));
      compute_done = boost::posix_time::microsec_clock::local_time();

      cycle_taken = compute_done - compute_start;
      cycle_remaining = boost::posix_time::microseconds(MIN_CYCLE_MICROSEC) - cycle_taken;

      boost::this_thread::sleep(cycle_remaining);

      cycle_done = boost::posix_time::microsec_clock::local_time();
      //cycle_total = cycle

      DEBUG_MESSAGE(boost::posix_time::to_simple_string(cycle_taken));
      DEBUG_VALUE_OF(compute_start);
      DEBUG_VALUE_OF(compute_done);
      DEBUG_VALUE_OF(cycle_done);

      DEBUG_VALUE_OF(cycle_taken);

      set_mission_accomplished(true);
    }

    return;
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_tollbooth_dlm() {
    DEBUG_METHOD();
    return string("can_call_tollbooth_dlm");
  }
}  // namespace decision

