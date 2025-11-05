#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <print>
#include <vector>

#include "sim/particle.h"
#include "sim/particles.h"
#include "sim/particles_aos.h"
#include "sim/vectors.h"
#include "utils/logger.h"
#include "utils/rng.h"

// To try the Array of Structs implementation
void ArrayOfStruct() {
  const size_t num_particles{100};
  RNG<Point3d> rng_points;
  std::vector<Point3d> points = rng_points.GenerateUniformRandom(
      num_particles, Point3d(-1, -1, -1), Point3d(1, 1, 1));
  RNG<Vec3d> rng_vecs;
  std::vector<Vec3d> velocities = rng_vecs.GenerateUniformRandom(
      num_particles, Vec3d(-10, -10, -10), Vec3d(10, 10, 10));
  RNG<double> rng_mass;
  std::vector<double> masses =
      rng_mass.GenerateUniformRandom(num_particles, 1, 200);

  std::vector<Particle> particles;
  particles.reserve(num_particles);

  for (size_t i = 0; i < num_particles; ++i) {
    particles.emplace_back(Particle(points[i], masses[i], velocities[i], i));
  }
  std::vector<Vec3d> F_ext = rng_vecs.GenerateUniformRandom(
      num_particles, Vec3d(-1, -5, -5), Vec3d(1, 5, 5));

  std::unique_ptr<ParticleSystemAoS> ps =
      std::make_unique<ParticleSystemAoS>(std::move(particles));

  const auto start{std::chrono::system_clock::now()};

  Vec3d F_global(0, -200, 0);
  DType d_t = 1;
  const size_t cycles{2};
  for (size_t steps = 0; steps < cycles; ++steps) {
    ps->UpdateN2(F_ext, F_global, d_t);
    // if (static_cast<long long>(t) % 5 == 0)
    // std::cout << *ps << std::endl;
  }
  const auto duration = std::chrono::system_clock::now() - start;
  std::cout << "Took "
            << std::chrono::round<std::chrono::milliseconds>(duration)
            << " for " << cycles << std::endl;
}

// TODO earth-sun particle system
/** Earth - Sun system,
 * with Sun at(0, 0, 0) and not moving.* earth doesn't move as expected
 */
void SunEarthSystem() {
  DType earth_to_sun{1.4904e11};
  Point3d pos_earth(earth_to_sun, 0, 0);
  DType earth_mass{5.972e24};
  Vec3d earth_v0(0, 29.78e3, 0);
  Particle earth(pos_earth, earth_mass, earth_v0, 0);

  Point3d pos_sun(0, 0, 0);
  DType sun_mass{1.989e30};
  Vec3d sun_v0(0, 0, 0);
  Particle sun(pos_sun, sun_mass, sun_v0, 1);

  std::vector<Particle> sun_earth{sun};
}

int main() {
  const size_t num_particles{4000};
  const double d_t{1.};
  Particles<> par_sys(num_particles, d_t);

  std::vector<double> Fext_x(num_particles);
  std::vector<double> Fext_y(num_particles);
  std::vector<double> Fext_z(num_particles);
#ifdef DEBUG
  std::fill_n(std::begin(Fext_x), num_particles, 0.);
  std::fill_n(std::begin(Fext_y), num_particles, 0.);
  std::fill_n(std::begin(Fext_z), num_particles, 0.);
#else
  RNG<double> rng;
  rng.GenerateUniformRandom(Fext_x.data(), num_particles, 0., 0.);
  rng.GenerateUniformRandom(Fext_y.data(), num_particles, 0., 0.);
  rng.GenerateUniformRandom(Fext_z.data(), num_particles, 0., 0.);
#endif
  auto start_the_clock = std::chrono::system_clock::now();
  for (size_t step = 0; step < 200; ++step) {
    par_sys.Update(Fext_x, Fext_y, Fext_z, 0., 0., 0.);
  }
  auto duration = std::chrono::system_clock::now() - start_the_clock;
  auto xx = std::chrono::round<std::chrono::milliseconds>(duration);
  std::println("Took: {}", xx);

  return 0;
}
