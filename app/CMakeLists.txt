# CMakeList.txt : CMake project for jsolver, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver_app")

file(GLOB_RECURSE SOURCES "${jsolver_app_SOURCE_DIR}/*.cpp")

add_executable (jsolver_app ${SOURCES})
target_link_libraries(jsolver_app PRIVATE jsolver_lib)