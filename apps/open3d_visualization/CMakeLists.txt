set(OPEN3D_VIZ_BINARY open3d_visualization)

add_executable(${OPEN3D_VIZ_BINARY} main.cpp)
target_compile_options(${OPEN3D_VIZ_BINARY} PRIVATE 
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:Debug>:-g;-O0;-fsanitize=address,undefined;-Wfloat-equal;-fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O3;-march=native;-DNDEBUG>)

target_link_libraries(${OPEN3D_VIZ_BINARY} PRIVATE particles_lib)# Open3D::Open3D)
target_link_options(${OPEN3D_VIZ_BINARY} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)