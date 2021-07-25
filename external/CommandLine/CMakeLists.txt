cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SOURCES "*.cpp")


message(STATUS "Build CommandLine: https://schneegans.github.io/")
add_library(CommandLine ${SOURCES})
target_include_directories(CommandLine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
