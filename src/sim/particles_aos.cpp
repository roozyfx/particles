#include <algorithm>

#include "sim/particles_aos.h"

ParticleSystemAoS::ParticleSystemAoS(const size_t n) { particles_.reserve(n); }
ParticleSystemAoS::ParticleSystemAoS(std::vector<Particle>&& particles) {
  particles_ = std::move(particles);
}

void ParticleSystemAoS::AddParticles(std::vector<Particle>&& particles) {
  std::move(particles.begin(), particles.end(), std::back_inserter(particles_));
}

void ParticleSystemAoS::UpdateN2(const std::vector<VectorType>& F_ext,
                                 const VectorType& F_global, const DType& d_t) {
  for (Particle& el : particles_) {
    el.Update(particles_, F_ext, F_global, d_t);
  }
}

std::ostream& operator<<(std::ostream& os, const ParticleSystemAoS& ps) {
  os << "=====Particle System=====" << std::endl;
  for (auto el : ps.particles_) {
    os << "{" << el.p << "}, m(" << el.m << "kg), →{" << el.v << "}"
       << std::endl;
  }
  os << "=========================" << std::endl;
  return os;
}
