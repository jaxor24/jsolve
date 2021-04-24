# CMakeList.txt : CMake project for tests, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver_tests")

# Add source to this project's executable.
add_executable (jsolver_tests "main.cpp" "test_matrix.cpp")

# TODO: Add tests and install targets if needed.

# target_link_libraries(jsolver PRIVATE spdlog::spdlog)