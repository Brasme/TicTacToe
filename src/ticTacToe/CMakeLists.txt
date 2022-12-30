cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToe3D)
project(${PROJECT_NAME} VERSION 0.0.2 DESCRIPTION "TicTacToe3D app" LANGUAGES CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ticTacToe3DLib)

if(BUILD_SHARED_LIBS)
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:ticTacToe3DLib> $<TARGET_FILE_DIR:${PROJECT_NAME}>    
)
endif()