// COPYRIGHT: Robosub Club of the Palouse 2013 under the GPL v3

#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "decision/base_dlm.h"
#include "decision/tollbooth_dlm.h"
#include "movement/fuzzy_sets.h"
#include "vision/base_eye.h"
#include "vision/vision_utilities.h"
#include "vision/tollbooth_eye.h"

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

