#include "particle.h"

#include <iostream>
#include <numeric>

#include "constants.hpp"

VectorType Particle::ForceFrom(const std::vector<Particle>& particles,
                               const std::vector<VectorType>& F_ext,
                               const VectorType& F_global) const noexcept {
  const DType Gm{G * m};
  VectorType Fext = id >= F_ext.size() ? VectorType(0, 0, 0) : F_ext.at(id);
  VectorType F = Fext + F_global;
  for (const auto& particle : particles) {
    // skip the current particle itself
    if (particle.id == id) continue;

    auto r{(particle.p - p)};
    using std::sqrt;
    DType epsilon2{DType(1e-12)};
    // softening
    auto denom{DType(1) / sqrt(r.Norm2() + epsilon2)};
    denom *= denom * denom;
    F += (-Gm) * particle.m * denom * r;
  }

  return F;
}

void Particle::Update(const std::vector<Particle>& particles,
                      const std::vector<VectorType>& F_ext,
                      const VectorType& F_global, const DType d_t) {
  auto F{ForceFrom(particles, F_ext, F_global)};
  auto a{F / m};
  UpdatePosition(a, d_t);
}
