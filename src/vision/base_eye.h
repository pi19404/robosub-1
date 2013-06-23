#ifndef SRC_VISION_BASE_EYE_H_
#define SRC_VISION_BASE_EYE_H_

#include <cv.h>
#include <highgui.h>
#include <string>

using ::std::string;

namespace vision {
  class BaseEye {
   protected:
    cv::Mat m_frame;
    cv::VideoCapture m_vidcap;

   public:
    BaseEye();
    BaseEye(cv::VideoCapture vidcap);
    virtual bool can_see_target()=0;
    virtual void blink();
  };

  string can_call_base_eye();
}  // namespace vision

#endif  // SRC_VISION_BASE_EYE_H_

