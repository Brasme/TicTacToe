cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME utils)
project(${PROJECT_NAME} VERSION 1.0.0 DESCRIPTION "Some common utils" LANGUAGES CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} 
	getopt.cpp  include/utils/getopt.h 
	tokens.cpp  include/utils/tokens.h 
	string.cpp  include/utils/string.h 
	lib.cpp     include/utils/lib.h)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE UTILS_VERSION="${PROJECT_VERSION}")
if (BUILD_SHARED_LIBS)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_SHARED)
endif()