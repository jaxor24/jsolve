# CMakeList.txt : CMake project for jsolver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (jsolver "main.cpp" "logging.cpp" "matrix.cpp" "test.cpp")

# TODO: Add tests and install targets if needed.

target_link_libraries(jsolver PRIVATE spdlog::spdlog)
