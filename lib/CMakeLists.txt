cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(jsolver_lib ${SOURCES})
set_target_properties(jsolver_lib PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

target_include_directories(jsolver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jsolver_lib PUBLIC spdlog)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")  	
	target_link_libraries(jsolver_lib PRIVATE tbb)
endif()
