#include "./fuzzy_sets.h"

#include <string>
using ::std::string;

namespace movement {
  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_fuzzy_sets() {
    return string("can_call_fuzzy_sets");
  }
}  // namespace decision

