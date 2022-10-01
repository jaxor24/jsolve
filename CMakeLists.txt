# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required(VERSION 3.16)

project(
	"jsolver" 
	VERSION 0.1 
	LANGUAGES CXX
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_VERBOSE_MAKEFILE ON)

	include(CTest)

	find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
	if (CLANG_TIDY_EXE)
		# Specify clang-tidy checks here
		set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*, clang-analyzer-*")
	endif()
endif()

add_subdirectory("external/CommandLine")
add_subdirectory("external/spdlog")

add_subdirectory("app")
add_subdirectory("lib")
add_subdirectory("data")
# add_subdirectory("benchmarks")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	find_package(TBB REQUIRED)
endif()

if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

