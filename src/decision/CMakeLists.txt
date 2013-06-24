add_library(decision_libs
    base_dlm.cpp driving_dlm.cpp mock_dlm.cpp
    parking_dlm.cpp path_dlm.cpp pizza_dlm.cpp speed_trap_dlm.cpp
    start_dlm.cpp tollbooth_dlm.cpp traffic_light_dlm.cpp
)

target_link_libraries(decision_libs movement_libs utility_libs vision_libs)
