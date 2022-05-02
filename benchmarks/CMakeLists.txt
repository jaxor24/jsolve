cmake_minimum_required (VERSION 3.8)

include(google/GoogleBenchmark.cmake)

add_executable (jsolver_bench "bench.cpp" "bench.h")

target_link_libraries(jsolver_bench PRIVATE benchmark::benchmark)
target_link_libraries(jsolver_bench PRIVATE jsolver_lib)

# Link Shlwapi to the project
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    target_link_libraries(jsolver_bench PRIVATE Shlwapi)
endif()

