find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

MACRO(add_benchmark benchmark_name)
  add_executable(${benchmark_name} ${benchmark_name}.cpp)
  target_link_libraries(${benchmark_name} PRIVATE particles_lib
    benchmark::benchmark
    benchmark::benchmark_main
    Threads::Threads
    tbb)

  target_include_directories(${benchmark_name} PRIVATE ../include)
  target_compile_options(${benchmark_name} PRIVATE -fsanitize=address -fno-omit-frame-pointer)

  target_link_options(${benchmark_name} PRIVATE -fsanitize=address)
ENDMACRO()

add_benchmark(benchmark_0)
