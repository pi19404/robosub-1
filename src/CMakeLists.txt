cmake_minimum_required (VERSION 2.8)
project (robosub)
add_library(robosub_pre_exe SHARED "")
add_executable(robosub main.cpp)
find_package(OpenCV REQUIRED)
#target_link_libraries(robosub ${OpenCV_LIBS} )
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
include_directories("${PROJECT_SOURCE_DIR}")
add_subdirectory (decision) 
add_subdirectory (movement) 
add_subdirectory (state) 
add_subdirectory (vision) 

target_link_libraries(robosub_pre_exe
  ${OpenCV_LIBS}
  decision_libs
  movement_libs
  state_libs
  vision_libs
)

