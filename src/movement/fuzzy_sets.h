#ifndef SRC_MOVEMENT_FUZZY_SETS_H_
#define SRC_MOVEMENT_FUZZY_SETS_H_

#include <string>
#include <boost/thread/mutex.hpp>
using ::std::string;

namespace movement {
  // Keep NUM_SUB_IS_VALUES as the last element.
  enum fuzzy_t {high, low, left, right, too_far_backward,
                too_far_forward, rotated_left, rotated_right, tilted_up,
                tilted_down, rolled_left, rolled_right,
                NUM_SETS};

  class FuzzySets {
   protected:
    double m_membership[NUM_SETS];

   public:
    FuzzySets();
    void set_sub_is(fuzzy_t category, double value);
    double get_sub_is(fuzzy_t category);

    boost::mutex& accessor_mut(int n) {
      static boost::mutex mut[NUM_SETS];
      return mut[n];
    }
  };

  string can_call_fuzzy_sets();
}  // namespace decision

#endif  // SRC_MOVEMENT_FUZZY_SETS_H_

