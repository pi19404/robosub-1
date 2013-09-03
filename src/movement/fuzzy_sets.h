// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#ifndef SRC_MOVEMENT_FUZZY_SETS_H_
#define SRC_MOVEMENT_FUZZY_SETS_H_

#include <boost/thread/mutex.hpp>
#include <string>

using ::std::string;

namespace movement {
  enum sub_wants_t {MOVE_DOWN, MOVE_UP, MOVE_RIGHT, MOVE_LEFT, MOVE_FORWARD,
                    MOVE_DOWNWARD, ROTATE_RIGHT, ROTATE_LEFT, TILT_DOWN,
                    TILT_UP, ROLL_RIGHT, ROLL_LEFT,
                    NUM_SETS};

  class FuzzySets {
   protected:
    double m_membership[NUM_SETS];
    boost::mutex& accessor_mut(int n);

   public:
    FuzzySets();
    void set_sub_wants_to(sub_wants_t category, double value);
    double get_sub_wants_to(sub_wants_t category);
  };

  string can_call_fuzzy_sets();
}  // namespace movement

#endif  // SRC_MOVEMENT_FUZZY_SETS_H_

