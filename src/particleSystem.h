#pragma once
#include <iostream>
#include <vector>

#include "particle.h"

class ParticleSystem {
  std::vector<Particle> particles_{};

 public:
  ParticleSystem(const size_t n);
  ParticleSystem(std::vector<Particle>&& particles);

  void AddParticles(std::vector<Particle>&& particles);

  void UpdateN2(const std::vector<VectorType>& F_ext,
                const VectorType& F_global, const DType& d_t);

  friend std::ostream& operator<<(std::ostream& os, const ParticleSystem& ps);
};