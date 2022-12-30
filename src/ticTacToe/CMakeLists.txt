cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToe3D)
project(${PROJECT_NAME} VERSION 0.0.2 DESCRIPTION "TicTacToe3D app" LANGUAGES CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ticTacToe3DLib)
