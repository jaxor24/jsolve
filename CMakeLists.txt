# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory ("external/CommandLine")
add_subdirectory ("external/spdlog")
add_subdirectory ("app")
add_subdirectory ("lib")
add_subdirectory ("data")
add_subdirectory ("tests")

if(NOT TARGET spdlog)
  find_package(spdlog REQUIRED)
  find_package(CommandLine REQUIRED)
endif()
