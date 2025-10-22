#include <iostream>
#include <memory>
#include <vector>

#include "particle.h"
#include "particleSystem.h"
#include "utils/logger.h"
#include "vectors.h"

int main() {
  Vec3d v0_all(0., 0., 0.);

  Point3d pos0(0, 0, 0);
  Vec3d v0(0., 0, 0.);
  DType m0{100000};
  Particle p0(pos0, m0, v0_all, 0);

  DType m{1000000};

  std::vector<Particle> particles;
  particles.emplace_back(p0);
  particles.emplace_back(Particle(Point3d(0.3, 0, 0), 2 * m, v0_all, 3));
  particles.emplace_back(Particle(Point3d(-1.2, 0, 0), m, v0_all, 4));
  particles.emplace_back(Particle(Point3d(0.5, 0.4, 0), m, v0_all, 5));
  particles.emplace_back(Particle(Point3d(0.1, -.2, 0), m, v0_all, 6));

  std::unique_ptr<ParticleSystem> ps =
      std::make_unique<ParticleSystem>(std::move(particles));

  Vec3d F0(0, 0, 0);
  std::vector<Vec3d> F_ext(particles.size(), F0);
  Vec3d F_global(0, 0, 0);
  DType d_t = 1;
  for (auto steps = 0; steps < 20; ++steps) {
    ps->UpdateN2(F_ext, F_global, d_t);
    // if (static_cast<long long>(t) % 5 == 0)
    std::cout << *ps << std::endl;
  }

  /*
   * Earth - Sun system,
   * with Sun at(0, 0, 0) and not moving.
   * earth doesn't move as expected
   */
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

  // @todo earth-sun particle system
  return 0;
}