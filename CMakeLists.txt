cmake_minimum_required(VERSION 3.60)

project(Particles)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g                \
                        -O0 -Wall -Wextra -Wpedantic            \
                        -fsanitize=address,undefined            \
                        -Wfloat-equal                           \
                        -fno-omit-frame-pointer")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -march=native -DNDEBUG")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ ")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weffc++")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions("DEBUG")
endif()

set(src 
    src/main.cpp 
    src/sim/particle.cpp 
    src/sim/particles_aos.cpp)

set(hdrs
    src/utils/logger.h
    src/sim/particle.h
    src/sim/particles.h
    src/sim/particles_rawpointer.h
    src/sim/particles_aos.h
    src/sim/linear_algebra.h
    src/sim/types.hpp
    src/sim/constants.hpp)

include_directories(src)

add_executable(${CMAKE_PROJECT_NAME} ${src} ${hdrs})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pthread)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DPARALLEL")
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE tbb)
endif()

enable_testing() 
add_subdirectory(tests) 
add_subdirectory(benchmark)
