add_library(vision_libs base_eye.cpp tollbooth_eye.cpp vision_utilities.cpp)
target_link_libraries(vision_libs ${OpenCV_LIBS})
