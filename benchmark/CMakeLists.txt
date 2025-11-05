find_package(benchmark REQUIRED)

# set(_GNU_SOURCE ON)
MACRO(add_bench benchmark_name)
  add_executable(${benchmark_name} ${benchmark_name}.cpp)
  target_link_libraries(${benchmark_name} PRIVATE
    benchmark::benchmark
    benchmark::benchmark_main)
ENDMACRO()

add_bench(benchmark_0)
