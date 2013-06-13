#include "base_dlm.h"
#include "tollbooth_eye.h"

#include <cv.h>
#include <highgui.h>
#include <string>

using ::std::string;

namespace decision {
  class TollboothDLM : public DecisionLogicModule {
   protected:
    vision::TollboothEye *m_forward_eye;
    vision::TollboothEye *m_downward_eye;

   public:
    TollboothDLM(movement::FuzzySets *fuzzy_sub_state,
                 cv::VideoCapture forward_vidcap,
                 cv::VideoCapture downward_vidcap)
    : DecisionLogicModule(fuzzy_sub_state, forward_vidcap, downward_vidcap) {
      m_forward_eye = new vision::TollboothEye();
      m_downward_eye = new vision::TollboothEye();
    }
  };

  string can_call_tollbooth_dlm();
}  // namespace decision

