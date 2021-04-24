# CMakeList.txt : CMake project for tests, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver_tests")

set(JSOLVER_TEST_SOURCES
    main.cpp
    test_matrix.cpp)

# Add source to this project's executable.
add_executable (jsolver_tests ${JSOLVER_TEST_SOURCES})
