# CMakeList.txt : CMake project for jsolver, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver_lib")

file(GLOB_RECURSE SOURCES "${jsolver_lib_SOURCE_DIR}/*.cpp")

add_library(jsolver_lib ${SOURCES})
target_link_libraries(jsolver_lib PUBLIC spdlog)
target_include_directories(jsolver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
