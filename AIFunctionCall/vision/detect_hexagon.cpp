#include "./detect_hexagon.h"

namespace vision {
  TollboothDetector::TollboothDetector(string filename) {
    m_frame = imread(filename);
  }

  TollboothDetector::TollboothDetector(Mat frame) {
    m_frame = frame;
  }

  TollboothDetector::~TollboothDetector() {
    m_frame.release();
  }

  // TODO: Implement
  bool TollboothDetector::can_see_tollbooth() {
    return false;
  }
}

// This is a throwaway stub used to
// confirm that gtest works.
int testing_gtest() {
  return 1337;
}

