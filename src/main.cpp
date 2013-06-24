#include <stdio.h>
#include <string.h>
#include <cv.h>
#include <highgui.h>
//#include <iostream>
#include "robosub.h"
#include "utility/DebugLog.hpp"
#include "state/state_machine.h"

#include "movement/fuzzy_sets.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  DEBUG_SET_STREAM(std::cerr);
  DEBUG_METHOD();

  if (argc == 2 && (strncmp(argv[1], "-v", 2) == 0 ||
                    strncmp(argv[1], "--version", 9) == 0)) {
    printf("%s Version %d.%d\n",
           argv[0], ROBOSUB_VERSION_MAJOR, ROBOSUB_VERSION_MINOR);
    return 0;
  }

  // TODO(LPE) An object (or several objects) that encapsulate the
  // Arduino communication protocols should be initialized here. The
  // StateMachine object will need to be modified to accept this
  // communication object.

  // TODO(LPE) These VideoCapture objects should refer to actual cameras.
  cv::VideoCapture forward_eye(0);
  cv::VideoCapture downward_eye(0);

  state::StateMachine sm(forward_eye, downward_eye);
  sm.activate();
}

