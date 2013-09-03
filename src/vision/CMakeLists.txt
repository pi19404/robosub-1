add_library(vision_libs
    vision_utilities.cpp base_eye.cpp driving_eye.cpp
    parking_eye.cpp path_eye.cpp pizza_eye.cpp speed_trap_eye.cpp
    start_eye.cpp tollbooth_eye.cpp traffic_light_eye.cpp
)

target_link_libraries(vision_libs utility_libs ${OpenCV_LIBS})
