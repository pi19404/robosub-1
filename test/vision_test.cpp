#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "tollbooth_eye.h"

using ::std::endl;
using ::std::vector;
using ::std::string;

class VisionTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    s_image_names = new vector<string>();
    s_image_names->push_back("old_images/old_bright_bgrp.jpg");
    s_image_names->push_back("old_images/old_bright_gbpr.jpg");
    s_image_names->push_back("old_images/old_bright_rpbg.jpg");
    s_image_names->push_back("old_images/old_dim_brgp.jpg");
    s_image_names->push_back("old_images/old_dim_prgb.jpg");
    s_image_names->push_back("old_images/old_norm_bgrp.jpg");
    s_image_names->push_back("old_images/old_norm_gbpr.jpg");
    s_image_names->push_back("old_images/old_norm_rpbg.jpg");
    s_image_names->push_back("old_images/old_perspective_white_back_rpbg.jpg");
    s_image_names->push_back("old_images/old_bright_brgp.jpg");
    s_image_names->push_back("old_images/old_bright_prgb.jpg");
    s_image_names->push_back("old_images/old_dim_bgrp.jpg");
    s_image_names->push_back("old_images/old_dim_gbpr.jpg");
    s_image_names->push_back("old_images/old_dim_rpbg.jpg");
    s_image_names->push_back("old_images/old_norm_brgp.jpg");
    s_image_names->push_back("old_images/old_norm_prgb.jpg");
    s_image_names->push_back("old_images/old_perspective_grey_back_rpbg.jpg");
  }

  static void TearDownTestCase() {
    delete s_image_names;
    s_image_names = NULL;
  }

  static vector<string>* s_image_names;
};

vector<string>* VisionTest::s_image_names = NULL;

// File names are expected to end in ABCD.jpg, where ABCD is
// some color code. If the leter is capitolized, the large hex
// should be toward the outside of the square. Otherwise, the
// small hex should be on the outside. The file names that start
// with old_ do not have a sufficient number of hexes.
string get_color_pattern(string filename) {
  int begin, end;
  end = filename.find(".jpg");
  begin = end - 4;
  return filename.substr(begin, 4);
}

// We should be able to get a string of length 4, and all 4 color chars
// should be in that string.
TEST_F(VisionTest, test_get_color_pattern) {
  long unsigned int locSmall, locLarge;
  for (vector<string>::iterator it = s_image_names->begin();
       it != s_image_names->end(); ++it) {
    ASSERT_EQ(get_color_pattern(*it).size(), 4U);
    locSmall = get_color_pattern(*it).find("b");
    locLarge = get_color_pattern(*it).find("B");
    ASSERT_TRUE(locSmall != string::npos || locLarge != string::npos);
    locSmall = get_color_pattern(*it).find("g");
    locLarge = get_color_pattern(*it).find("G");
    ASSERT_TRUE(locSmall != string::npos || locLarge != string::npos);
    locSmall = get_color_pattern(*it).find("r");
    locLarge = get_color_pattern(*it).find("R");
    ASSERT_TRUE(locSmall != string::npos || locLarge != string::npos);
    locSmall = get_color_pattern(*it).find("p");
    locLarge = get_color_pattern(*it).find("P");
    ASSERT_TRUE(locSmall != string::npos || locLarge != string::npos);
  }
}

// When a TollboothDetector is instantiated with a Mat that has a
// tollbooth image, it should claim that it can see it.
// TODO: Add some negative tests.
TEST_F(VisionTest, test_can_see_tollbooth) {
  for (vector<string>::iterator it = s_image_names->begin();
       it != s_image_names->end(); ++it) {
    vision::TollboothDetector *uut = new vision::TollboothDetector(*it);
    ASSERT_TRUE(uut->can_see_tollbooth())
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

