cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToe3DLib)
project(${PROJECT_NAME} VERSION 1.0.0 DESCRIPTION "TicTacToe3D library" LANGUAGES CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} 
	color.cpp   include/ticTacToe3D/color.h 
	field.cpp   include/ticTacToe3D/field.h 
	board.cpp   include/ticTacToe3D/board.h 
	lib.cpp     include/ticTacToe3D/lib.h)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE TTT_VERSION="${PROJECT_VERSION}")
if (BUILD_SHARED_LIBS)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_SHARED)
endif()