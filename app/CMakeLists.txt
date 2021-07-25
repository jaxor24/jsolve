cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable (jsolver_app ${SOURCES})
target_link_libraries(jsolver_app PRIVATE jsolver_lib)
target_link_libraries(jsolver_app PRIVATE jsolver_data)
target_link_libraries(jsolver_app PRIVATE CommandLine)