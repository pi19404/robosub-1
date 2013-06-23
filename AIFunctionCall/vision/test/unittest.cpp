#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "../detect_hexagon.h"

using ::std::endl;
using ::std::vector;
using ::std::string;

class VisionTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    s_image_names = new vector<string>();
    s_image_names->push_back("bright_bgrp.jpg");
    s_image_names->push_back("bright_gbpr.jpg");
    s_image_names->push_back("bright_rpbg.jpg");
    s_image_names->push_back("dim_brgp.jpg");
    s_image_names->push_back("dim_prgb.jpg");
    s_image_names->push_back("norm_bgrp.jpg");
    s_image_names->push_back("norm_gbpr.jpg");
    s_image_names->push_back("norm_rpbg.jpg");
    s_image_names->push_back("perspective_white_back_rpbg.jpg");
    s_image_names->push_back("bright_brgp.jpg");
    s_image_names->push_back("bright_prgb.jpg");
    s_image_names->push_back("dim_bgrp.jpg");
    s_image_names->push_back("dim_gbpr.jpg");
    s_image_names->push_back("dim_rpbg.jpg");
    s_image_names->push_back("norm_brgp.jpg");
    s_image_names->push_back("norm_prgb.jpg");
    s_image_names->push_back("perspective_grey_back_rpbg.jpg");
  }

  static void TearDownTestCase() {
    delete s_image_names;
    s_image_names = NULL;
  }

  static vector<string>* s_image_names;
};

vector<string>* VisionTest::s_image_names = NULL;

string get_color_pattern(string filename) {
  int begin, end;
  end = filename.find(".jpg");
  begin = end - 4;
  return filename.substr(begin, 4);
}

TEST(misc_tests, confirm_paths_work) {
  EXPECT_EQ(testing_gtest(), 1337);
}

// We should be able to get a string of length 4, and all 4 color chars
// should be in that string.
TEST_F(VisionTest, test_get_color_pattern) {
  long unsigned int locSmall, locLarge;
  for (vector<string>::iterator it = s_image_names->begin();
       it != s_image_names->end(); ++it) {
    EXPECT_EQ(get_color_pattern(*it).size(), 4U);
    locSmall = get_color_pattern(*it).find("b");
    locLarge = get_color_pattern(*it).find("B");
    EXPECT_TRUE(locSmall != string::npos || locLarge != string::npos);
    locSmall = get_color_pattern(*it).find("g");
    locLarge = get_color_pattern(*it).find("G");
    EXPECT_TRUE(locSmall != string::npos || locLarge != string::npos);
    locSmall = get_color_pattern(*it).find("r");
    locLarge = get_color_pattern(*it).find("R");
    EXPECT_TRUE(locSmall != string::npos || locLarge != string::npos);
    locSmall = get_color_pattern(*it).find("p");
    locLarge = get_color_pattern(*it).find("P");
    EXPECT_TRUE(locSmall != string::npos || locLarge != string::npos);
  }
}

// When a TollboothDetector is instantiated with a Mat that has a
// tollbooth image, it should claim that it can see it.
// TODO: Add some negative tests.
TEST_F(VisionTest, test_can_see_tollbooth) {
  for (vector<string>::iterator it = s_image_names->begin();
       it != s_image_names->end(); ++it) {
    vision::TollboothDetector *uut = new vision::TollboothDetector(*it);
    EXPECT_TRUE(uut->can_see_tollbooth())
        << "Failed on filename: " << *it << endl;
    delete uut;
  }
}

TEST_F(VisionTest, test_get_tollbooth_bounding_box) {
  FAIL() << "Not implemented." << endl;
}

TEST_F(VisionTest, test_get_tollbooth_flattened_rect) {
  FAIL() << "Not implemented." << endl;
}

TEST_F(VisionTest, test_get_tollbooth_windows) {
  FAIL() << "Not implemented." << endl;
}

TEST_F(VisionTest, test_get_tollbooth_window_color) {
  FAIL() << "Not implemented." << endl;
}

TEST_F(VisionTest, test_get_tollbooth_window_hex_center) {
  FAIL() << "Not implemented." << endl;
}

