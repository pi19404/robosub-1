cmake_minimum_required (VERSION 2.6)
project (robosub)
add_executable(robosub main.cpp)
find_package(OpenCV REQUIRED)
target_link_libraries(robosub ${OpenCV_LIBS} )
# The version number.
set (ROBOSUB_VERSION_MAJOR 0)
set (ROBOSUB_VERSION_MINOR 1)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/robosub.h.in"
  "${PROJECT_BINARY_DIR}/robosub.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find robosubConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/decision")
include_directories ("${PROJECT_SOURCE_DIR}/movement")
include_directories ("${PROJECT_SOURCE_DIR}/state_machine")
include_directories ("${PROJECT_SOURCE_DIR}/vision")
add_subdirectory (decision) 
add_subdirectory (movement) 
add_subdirectory (state_machine) 
add_subdirectory (vision) 

# decision
target_link_libraries (robosub base_dlm) 
target_link_libraries (robosub tollbooth_dlm) 
# movement
target_link_libraries (robosub fuzzy_sets) 
# vision
target_link_libraries (robosub base_eye) 
target_link_libraries (robosub tollbooth_eye) 
target_link_libraries (robosub vision_utilities) 

