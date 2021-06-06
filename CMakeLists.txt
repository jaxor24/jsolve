# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.8)

project ("jsolver_v0")

add_subdirectory ("external/spdlog")
add_subdirectory ("app")
add_subdirectory ("lib")
add_subdirectory ("data")
add_subdirectory ("tests")

if(NOT TARGET spdlog)
  find_package(spdlog REQUIRED)
endif()
