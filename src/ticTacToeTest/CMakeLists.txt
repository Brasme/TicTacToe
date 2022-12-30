cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToe3DTest)
project(${PROJECT_NAME} CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

enable_testing()
add_executable(${PROJECT_NAME} board_test.cpp)
target_link_libraries(${PROJECT_NAME} ticTacToe3DLib gtest gtest_main)
