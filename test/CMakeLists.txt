cmake_minimum_required(VERSION 2.6)
project(robosub_tests)
enable_testing()
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

# Assumptions:
# 1) The environment variable GTEST_ROOT is set
# 2) A $GTEST_ROOT/build folder exists
# 3) The standard "cmake ..; make" command was run from inside of that folder.
message(STATUS GTEST_ROOT=$ENV{GTEST_ROOT})
set (ROBOSUB_SOURCE_DIR ${META_PROJECT_SOURCE_DIR}/src)
message(STATUS ROBOSUB_SOURCE_DIR = ${ROBOSUB_SOURCE_DIR})
include_directories($ENV{GTEST_ROOT})
link_directories($ENV{GTEST_ROOT}/build)
include_directories($ENV{GTEST_ROOT}/include $ENV{GTEST_ROOT})
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${ROBOSUB_SOURCE_DIR})

include_directories("${ROBOSUB_SOURCE_DIR}/decision")
include_directories("${ROBOSUB_SOURCE_DIR}/movement")
include_directories("${ROBOSUB_SOURCE_DIR}/state_machine")
include_directories("${ROBOSUB_SOURCE_DIR}/vision")

add_executable(robosub_tests vision_test.cpp can_call.cpp)

target_link_libraries(robosub_tests robosub_pre_exe)
target_link_libraries(robosub_tests gtest gtest_main)
target_link_libraries(robosub_tests ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME robosub_tests COMMAND robosub_tests)

