# CMakeList.txt : CMake project for tests, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver_tests")

file(GLOB_RECURSE SOURCES "${jsolver_tests_SOURCE_DIR}/*.cpp")

add_executable (jsolver_tests ${SOURCES})
target_link_libraries(jsolver_tests PRIVATE jsolver_lib)
