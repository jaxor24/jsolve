cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(jsolver_lib ${SOURCES})
target_include_directories(jsolver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jsolver_lib PUBLIC spdlog)
