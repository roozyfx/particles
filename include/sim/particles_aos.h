#pragma once

#include <vector>

#include "particle.h"
/**
 * A Particle System class, as an Array of Struct of Particle particles.
 * This is mainly for test and sanity check on the physics
 */
class ParticleSystemAoS {
  std::vector<Particle> particles_{};

 public:
  ParticleSystemAoS(const size_t n);
  ParticleSystemAoS(std::vector<Particle>&& particles);

  void AddParticles(std::vector<Particle>&& particles);

  void UpdateN2(const std::vector<VectorType>& F_ext,
                const VectorType& F_global, const DType& d_t);

  friend std::ostream& operator<<(std::ostream& os,
                                  const ParticleSystemAoS& ps);
};
