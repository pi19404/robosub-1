#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
// src/decision
#include "base_dlm.h"
#include "tollbooth_dlm.h"
// src/movement
#include "fuzzy_sets.h"
// src/state_machine
// src/vision
#include "base_eye.h"
#include "vision_utilities.h"
#include "tollbooth_eye.h"

using ::std::string;

////////////////////
// decision files //
////////////////////

TEST(CanCallTests, base_dlm) {
  ASSERT_EQ(decision::can_call_base_dlm(), string("can_call_base_dlm"));
}

TEST(CanCallTests, tollbooth_dlm) {
  ASSERT_EQ(decision::can_call_tollbooth_dlm(), string("can_call_tollbooth_dlm"));
}

////////////////////
// movement files //
////////////////////

TEST(CanCallTests, fuzzy_sets) {
  ASSERT_EQ(movement::can_call_fuzzy_sets(), string("can_call_fuzzy_sets"));
}

/////////////////////////
// state_machine files //
/////////////////////////

//////////////////
// vision files //
//////////////////
TEST(CanCallTests, base_eye) {
  ASSERT_EQ(vision::can_call_base_eye(), string("can_call_base_eye"));
}

TEST(CanCallTests, vision_utilities) {
  ASSERT_EQ(vision::can_call_vision_utilities(), string("can_call_vision_utilities"));
}

TEST(CanCallTests, tollbooth_eye) {
  ASSERT_EQ(vision::can_call_tollbooth_eye(), string("can_call_tollbooth_eye"));
}

