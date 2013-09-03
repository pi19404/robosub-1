// COPYRIGHT: Robosub Club of the Palouse under the GPL v3

#include "decision/base_dlm.h"
#include <string>
#include "utility/DebugLog.hpp"

using ::std::string;

namespace decision {
  void DecisionLogicModule::set_mission_accomplished(bool setting) {
    DEBUG_METHOD();
    m_mission_accomplished = setting;
  }

  bool DecisionLogicModule::get_mission_accomplished() {
    return m_mission_accomplished;
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_base_dlm() {
    DEBUG_METHOD();
    return string("can_call_base_dlm");
  }
}  // namespace decision

