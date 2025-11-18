cmake_minimum_required(VERSION 3.60)

project(Particles)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions("DEBUG")
endif()

find_package(Threads REQUIRED)

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

set(BINARY_TARGET particles)
add_executable(${BINARY_TARGET} ${src} ${hdrs})
target_compile_options(${BINARY_TARGET} PRIVATE -pthread)
target_compile_options(${BINARY_TARGET} PRIVATE 
                        -Wall -Wextra -Wpedantic
                        $<$<CONFIG:Debug>:-g;-fsanitize=address,undefined;-O0;-Wfloat-equal;-fno-omit-frame-pointer>
                        $<$<CONFIG:Release>:-O3;-march=native;-DNDEBUG>)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${BINARY_TARGET} -stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${BINARY_TARGET} PRIVATE 
                        $<$<CONFIG:Debug>:-Weffc++>)
endif()

target_link_options(${BINARY_TARGET} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

target_link_libraries(${BINARY_TARGET} PRIVATE Threads::Threads)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DPARALLEL")
  target_link_libraries(${BINARY_TARGET} PRIVATE tbb)
endif()

enable_testing() 
add_subdirectory(tests) 
add_subdirectory(benchmark)
