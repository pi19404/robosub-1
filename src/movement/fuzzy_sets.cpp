#include "./fuzzy_sets.h"

#include <string>
#include <boost/thread/mutex.hpp>

using ::std::string;

namespace movement {
  FuzzySets::FuzzySets() {
    for (int i = 0; i < NUM_SETS; i++) {
      m_membership[i] = 0.0;
    }
  }

  void FuzzySets::set_sub_is(fuzzy_t category, double membership) {
    boost::lock_guard<boost::mutex> (accessor_mut(category));
    assert(0.0 <= membership && membership <= 1.0);
    m_membership[category] = membership;
  }

  double FuzzySets::get_sub_is(fuzzy_t category) {
    boost::lock_guard<boost::mutex> (accessor_mut(category));
    return m_membership[category];
  }

  boost::mutex& accessor_mut(int n) {
    static boost::mutex mut[NUM_SETS];
    return mut[n];
  }

  // This function exists to make sure we can call functions
  // in this file from the testing dir. Don't remove.
  string can_call_fuzzy_sets() {
    return string("can_call_fuzzy_sets");
  }
}  // namespace decision

