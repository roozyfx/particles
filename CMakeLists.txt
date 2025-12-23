cmake_minimum_required(VERSION 3.26..4.2)

project(Particles VERSION 0.1.0
                        DESCRIPTION "Particle system simulation"
                        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions("DEBUG")
endif()

# Options
option(BUILD_TESTING "Build tests" ON)
option(BUILD_BENCHMARK "Build benchmarks" ON)

add_subdirectory(src)
add_subdirectory(apps)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests) 
endif()
if(BUILD_TESTING)
  add_subdirectory(benchmark)
endif()
