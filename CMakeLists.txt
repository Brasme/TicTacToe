cmake_minimum_required(VERSION 3.10)

project(TicTacToe3DBundle)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest" OFF)
option(GTEST_HAS_ABSL "Use Abseil and RE2 in gtest" OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(modules/googletest)
add_subdirectory(src)
