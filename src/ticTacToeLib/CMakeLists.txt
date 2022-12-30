cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToeLib)
project(${PROJECT_NAME} CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} board.cpp include/ticTacToe/board.h include/ticTacToe/lib.h)
target_compile_definitions()