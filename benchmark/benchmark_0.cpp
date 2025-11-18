#include <benchmark/benchmark.h>

#include "../src/sim/particles.h"
#include "../src/utils/logger.h"
#include "../src/utils/rng.h"

// Define benchmark
static void BM_ParticleUpdate(benchmark::State& state) {
  const size_t num_particles{4000};
  const double d_t{1.};
  Particles<> par_sys(num_particles, d_t);

  std::vector<double> Fext_x(num_particles);
  std::vector<double> Fext_y(num_particles);
  std::vector<double> Fext_z(num_particles);

  RNG<double> rng;
  rng.GenerateUniformRandom(Fext_x.data(), num_particles, 0., 0.);
  rng.GenerateUniformRandom(Fext_y.data(), num_particles, 0., 0.);
  rng.GenerateUniformRandom(Fext_z.data(), num_particles, 0., 0.);
  for (auto _ : state) {
    for (size_t step = 0; step < 200; ++step) {
      par_sys.Update(Fext_x, Fext_y, Fext_z, 0., 0., 0.);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_ParticleUpdate);

BENCHMARK_MAIN();
