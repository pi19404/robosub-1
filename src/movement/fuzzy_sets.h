#ifndef SRC_MOVEMENT_FUZZY_SETS_H_
#define SRC_MOVEMENT_FUZZY_SETS_H_

#include <string>
#include <boost/thread/mutex.hpp>

using ::std::string;

namespace movement {
  enum sub_wants_t {move_down, move_up, move_right, move_left, move_forward,
                    move_downward, rotate_right, rotate_left, tilt_down,
                    tilt_up, roll_right, roll_left,
                    NUM_SETS};

  class FuzzySets {
   protected:
    double m_membership[NUM_SETS];

   public:
    FuzzySets();
    void set_sub_wants_to(sub_wants_t category, double value);
    double get_sub_wants_to(sub_wants_t category);

    boost::mutex& accessor_mut(int n);
  };

  string can_call_fuzzy_sets();
}  // namespace movement

#endif  // SRC_MOVEMENT_FUZZY_SETS_H_

