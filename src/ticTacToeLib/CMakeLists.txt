cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToe3DLib)
project(${PROJECT_NAME} VERSION 0.0.1 DESCRIPTION "TicTacToe3D library" LANGUAGES CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} board.cpp lib.cpp include/ticTacToe3D/board.h include/ticTacToe3D/lib.h)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE TTT_VERSION="${PROJECT_VERSION}")
if (BUILD_SHARED_LIBS)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_SHARED)
endif()