# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("jsolver")
add_subdirectory ("external/spdlog")
add_subdirectory ("jsolver")

if(NOT TARGET spdlog)
  set(spdlog_DIR "H:\\jr_repos\\jsolver\\external\\spdlog\\")
  find_package(spdlog REQUIRED)
endif()

# Include sub-projects.



