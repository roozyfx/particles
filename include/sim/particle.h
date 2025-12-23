#pragma once
#include <limits>
#include <vector>

#include "linear_algebra.h"

using PointType = Point3d;
using VectorType = Vec3d;

/** For such a basic type, I find it that it is only redundant to
 * hide data members. So, I've made this a struct with direct
 * access to position, mass, velocity and .
 */
struct Particle {
  // position
  PointType p;
  // mass
  DType m{0.001f};
  // velocity
  VectorType v{0.f, 0.0f, 0.0f};

  size_t id;

  Particle(const PointType position, const DType mass,
           const VectorType velocity, const size_t idx)
      : p{position}, m{mass}, v{velocity}, id{idx} {}

  Particle(const Particle&) = default;
  Particle& operator=(const Particle&) = default;

  bool constexpr IsAlmostEqual(
      const Particle& other,
      DType epsilon = std::numeric_limits<DType>::epsilon()) const noexcept {
    return p.IsAlmostEqual(other.p) && std::abs(m - other.m) < epsilon &&
           v.IsAlmostEqual(other.v);
  }

  /**
   * @brief Updates this particle's velocity and position, using a semi-Euler
   * method
   * @param particles Vector of particles whose pairwise interactions with
   * this particle are considered.
   * @param F_ext External force per particle. Must be the same size and
   * index-aligned with \p particles so that F_ext[i] applies to particles[i].
   * If this is not the case, that F_ext is ignored.
   * @param F_global  Global force applied to all particles, e.g. gravity.
   */
  void Update(const std::vector<Particle>& particles,
              const std::vector<VectorType>& F_ext, const VectorType& F_global,
              const DType d_t);

 private:
  /**
   * @brief Semi-Euler update on this particle's velocity and position.
   */
  inline void UpdatePosition(const VectorType& a /*acceleration*/,
                             const DType d_t /*time*/) {
    v += d_t * a;
    p += d_t * v;
  }

  /**
   * @brief Compute the net force acting on this particle.
   *
   * Calculates the total force on this Particle by summing:
   *  - pairwise interaction forces between this particle and each element of \p
   * particles (if the container contains this particle instance,
   * self-interaction is ignored),
   *  - the per-particle external force taken from \p F_ext at the index
   * corresponding to the same entry in \p particles,
   *  - the additional global force \p F_global applied to this particle.
   *
   * @param particles Vector of particles whose pairwise interactions with this
   * particle are considered.
   * @param F_ext     External force per particle. Must be the same size and
   * index-aligned with \p particles so that F_ext[i] applies to particles[i].
   * If this is not the case, that F_ext is ignored.
   * @param F_global  Global force applied to all particles, e.g. gravity.
   *
   * @return The resulting force as a VectorType:
   * F = -G m1.m2 r / |r|³ + F_ext[i] + F_global
   *
   * @note Complexity: O(n) in the number of particles.
   */
  VectorType ForceFrom(const std::vector<Particle>& particles,
                       const std::vector<VectorType>& F_ext,
                       const VectorType& F_global) const noexcept;
};
