add_library(state_libs state_machine.cpp)
target_link_libraries(state_libs ${OpenCV_LIBS})
target_link_libraries(state_libs decision_libs movement_libs utility_libs vision_libs)
