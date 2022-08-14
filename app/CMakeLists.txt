cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(jsolver_app ${SOURCES})
set_target_properties(jsolver_app PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

target_link_libraries(jsolver_app PRIVATE jsolver_lib)
target_link_libraries(jsolver_app PRIVATE jsolver_data)
target_link_libraries(jsolver_app PRIVATE CommandLine)