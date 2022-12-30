cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ticTacToe3DTest)
project(${PROJECT_NAME} CXX)

include(CMakeDependentOption)
include(GNUInstallDirs)

enable_testing()
add_executable(${PROJECT_NAME} board_test.cpp lib_test.cpp)
target_link_libraries(${PROJECT_NAME} ticTacToe3DLib gtest gtest_main)

if(BUILD_SHARED_LIBS)
add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:ticTacToe3DLib> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
endif()