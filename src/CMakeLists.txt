
set(PROJECT_LIBRARY_NAME particles_lib)

find_package(Threads REQUIRED)

set(src 
    sim/particle_structure.cpp 
    sim/aos_particle_system.cpp
    utils/rng.cpp)

add_library(${PROJECT_LIBRARY_NAME} STATIC ${src})

target_compile_options(${PROJECT_LIBRARY_NAME} PRIVATE 
                        -pthread -Wall -Wextra -Wpedantic
                        $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
                        $<$<CONFIG:Release>:-O3;-march=native;-DNDEBUG>)

target_include_directories(${PROJECT_LIBRARY_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${PROJECT_LIBRARY_NAME} -stdlib=libc++)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${PROJECT_LIBRARY_NAME} PUBLIC 
                        $<$<CONFIG:Debug>:-Weffc++>)
endif()

target_link_options(${PROJECT_LIBRARY_NAME} PUBLIC
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

target_link_libraries(${PROJECT_LIBRARY_NAME} PUBLIC Threads::Threads)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DPARALLEL")
  target_link_libraries(${PROJECT_LIBRARY_NAME} PRIVATE tbb)
endif()
