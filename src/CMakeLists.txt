cmake_minimum_required(VERSION 2.8)
project(robosub)
add_library(robosub_pre_exe SHARED "")
add_executable(robosub main.cpp)
target_link_libraries(robosub robosub_pre_exe)
find_package(OpenCV REQUIRED)
#target_link_libraries(robosub ${OpenCV_LIBS} )
# The version number.
set(ROBOSUB_VERSION_MAJOR 0)
set(ROBOSUB_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/robosub.h.in"
  "${PROJECT_BINARY_DIR}/robosub.h"
)

option(USE_DEBUG_LOGGING
  "Print debug logs to stderr on execution." ON
)

if(USE_DEBUG_LOGGING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_LOG_ENABLE")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_LOG_ENABLE")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DDEBUG_LOG_ENABLE")
endif(USE_DEBUG_LOGGING)

# add the binary tree to the search path for include files
# so that we will find robosubConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
add_subdirectory(decision)
add_subdirectory(movement)
add_subdirectory(state)
add_subdirectory(utility)
add_subdirectory(vision)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(robosub_pre_exe
  ${OpenCV_LIBS}
  decision_libs
  movement_libs
  state_libs
  utility_libs
  vision_libs
  ${CMAKE_THREAD_LIBS_INIT}
  ${BOOST_LIBRARIES}
)

