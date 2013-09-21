#include "gtest/gtest.h"
#include "movement/fuzzy_sets.h"

// We should be able to set and get values for the fuzzy sets.
// This does not confirm that the mutexes are working correctly.
TEST(MovementTests, test_get_set_sub_wants) {
  movement::FuzzySets uut = movement::FuzzySets();
  movement::sub_wants_t desire;
  for (int i = 0; i < movement::NUM_SETS; i++) {
    desire = static_cast<movement::sub_wants_t>(i);
    ASSERT_TRUE(uut.get_sub_wants_to(desire) == 0.0);
    uut.set_sub_wants_to(desire, 0.5);
    ASSERT_TRUE(uut.get_sub_wants_to(desire) == 0.5);
  }
}

