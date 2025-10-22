cmake_minimum_required(VERSION 3.26)

PROJECT(Particles)
SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g                \
                        -O0 -Wall -Wextra -Wpedantic            \
                        -Weffc++ -fsanitize=address,undefined   \
                        -Wfloat-equal                           \
                        -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")

set(src 
    main.cpp

    src/particle.cpp
    src/particleSystem.cpp
    src/point.cpp
)

set(hdrs
    src/utils/logger.h

    src/particle.h
    src/particleSystem.h
    src/point.h
    src/vectors.h
    src/types.hpp
)

include_directories(src)

add_executable(${CMAKE_PROJECT_NAME} ${src} ${hdrs})
target_link_libraries(${CMAKE_PROJECT_NAME} pthread)

enable_testing()
add_subdirectory(tests)