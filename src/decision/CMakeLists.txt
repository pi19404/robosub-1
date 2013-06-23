add_library(decision_libs base_dlm.cpp tollbooth_dlm.cpp mock_dlm.cpp)
target_link_libraries(decision_libs movement_libs vision_libs)
