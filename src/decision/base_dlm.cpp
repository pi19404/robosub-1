#include "./base_dlm.h"

#include <string>
using ::std::string;

namespace decision {
  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_base_dlm() {
    return string("can_call_base_dlm");
  }
}  // namespace decision

