cmake_minimum_required(VERSION 2.6)
project(robosub_tests)
enable_testing()
find_package(Threads)

# Assumptions:
# 1) The environment variable GTEST_ROOT is set
# 2) A $GTEST_ROOT/build folder exists
# 3) The standard "cmake ..; make" command was run from inside of that folder.
message(STATUS GTEST_ROOT=$ENV{GTEST_ROOT})
include_directories($ENV{GTEST_ROOT})
link_directories($ENV{GTEST_ROOT}/build)
include_directories($ENV{GTEST_ROOT}/include $ENV{GTEST_ROOT})
include_directories("${PROJECT_BINARY_DIR}")

set (ROBOSUB_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../src)
include_directories ("${ROBOSUB_SOURCE_DIR}/decision")
include_directories ("${ROBOSUB_SOURCE_DIR}/movement")
include_directories ("${ROBOSUB_SOURCE_DIR}/state_machine")
include_directories ("${ROBOSUB_SOURCE_DIR}/vision")
add_subdirectory (decision) 
add_subdirectory (movement) 
add_subdirectory (state_machine) 
add_subdirectory (vision) 

# decision
target_link_libraries (robosub_tests base_dlm) 
target_link_libraries (robosub_tests tollbooth_dlm) 
# movement
target_link_libraries (robosub_tests fuzzy_sets) 
# vision
target_link_libraries (robosub_tests base_eye) 
target_link_libraries (robosub_tests tollbooth_eye) 
target_link_libraries (robosub_tests vision_utilities) 

add_executable(robosub_tests
  foobar_test.cpp
)
target_link_libraries(robosub_tests gtest gtest_main)
target_link_libraries(robosub_tests ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME robosub_tests COMMAND robosub_tests)

