#pragma once

#include <cmath>
#include <concepts>
#include <cstdio>
#include <iostream>
#include <vector>

#include "../utils/rng.h"
#include "constants.hpp"

// TODO: Benchmark and test between using std::vector class, Particles and the
// raw pointer implementation ParticlesRawPointer
/**
 * A Particle System, as a Struct of Arrays of properties
 * Using raw pointers.
 */

template <std::floating_point T = double>
struct ParticlesRawPointer {
  // number of particles
  size_t n;
  T d_t;
  // positions
  T *x, *y, *z;
  // mass
  T* m;
  T* Gm;
  // velocity
  T *vx, *vy, *vz;

  // acceleration, Beschleunigung
  T *ax, *ay, *az;
  // Force on each particle. This is temporary, if I choose Gravity between
  // particles as a the simulated force.
  T *Fx, *Fy, *Fz;
  static T constexpr epsilon{T(0.0001)};
  static T constexpr eps2{epsilon * epsilon};

  ParticlesRawPointer(const size_t n, const T d_t)
      : n{n},
        d_t{d_t},
        x{new T[n]},
        y{new T[n]},
        z{new T[n]},
        m{new T[n]},
        Gm{new T[n]},
        vx{new T[n]},
        vy{new T[n]},
        vz{new T[n]},
        ax{new T[n]},
        ay{new T[n]},
        az{new T[n]},
        Fx{new T[n]},
        Fy{new T[n]},
        Fz{new T[n]} {
    Randomize();
    for (size_t i = 0; i < n; ++i) Gm[i] = G * m[i];
  }

  ParticlesRawPointer(const ParticlesRawPointer&) = delete;
  ParticlesRawPointer& operator=(const ParticlesRawPointer&) = delete;
  ParticlesRawPointer(ParticlesRawPointer&&) noexcept = delete;
  ParticlesRawPointer& operator=(ParticlesRawPointer&&) noexcept = delete;
  ~ParticlesRawPointer() {
    if (x) delete[] x;
    if (y) delete[] y;
    if (z) delete[] z;
    if (m) delete[] m;
    if (Gm) delete[] Gm;
    if (vx) delete[] vx;
    if (vy) delete[] vy;
    if (vz) delete[] vz;
    if (ax) delete[] ax;
    if (ay) delete[] ay;
    if (az) delete[] az;
    if (Fx) delete[] Fx;
    if (Fy) delete[] Fy;
    if (Fz) delete[] Fz;
  }

  /**
   * @brief Updates this particle's velocity and position, using a semi-Euler
   * method
   *
   */
  // @todo: Make different options for Force, F. such as gravity between
  // particles, no inner-force just initial velocity and global (earth) gravity,
  // etc.
  void Update(const T* F_ext_x, const T* F_ext_y, const T* F_ext_z,
              const T F_global_x, const T F_global_y, const T F_global_z) {
    UpdateForces(F_ext_x, F_ext_y, F_ext_z, F_global_x, F_global_y, F_global_z);

    UpdateState();
  }

 private:
  void UpdateForces(const T* F_ext_x, const T* F_ext_y, const T* F_ext_z,
                    const T F_global_x, const T F_global_y,
                    const T F_global_z) {
    std::fill_n(Fx, n, T(0));
    std::fill_n(Fy, n, T(0));
    std::fill_n(Fz, n, T(0));

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        const T rx{x[j] - x[i]};
        const T ry{y[j] - y[i]};
        const T rz{z[j] - z[i]};

        const T r2{rx * rx + ry * ry + rz * rz + eps2};
        const T inv_r2{T(1.) / r2};
        using std::sqrt;
        T inv_r = sqrt(inv_r2);

        T F = Gm[i] * m[j] * inv_r2 * inv_r;
        T fx{F * rx};
        T fy{F * ry};
        T fz{F * rz};

        // Update forces
        Fx[i] += fx;
        Fx[j] -= fx;
        Fy[i] += fy;
        Fy[j] -= fy;
        Fz[i] += fz;
        Fz[j] -= fz;
      }
    }

    // Add external Force to each particle, if exists
    if (F_ext_x)
      for (size_t i = 0; i < n; ++i) Fx[i] += F_ext_x[i];
    if (F_ext_y)
      for (size_t i = 0; i < n; ++i) Fy[i] += F_ext_y[i];
    if (F_ext_z)
      for (size_t i = 0; i < n; ++i) Fz[i] += F_ext_z[i];
    // Add global force
    for (size_t i = 0; i < n; ++i) {
      Fx[i] += F_global_x;
      Fy[i] += F_global_y;
      Fz[i] += F_global_z;
    }
  }

  /**
   * @brief Semi-Euler update on this particle's velocity and position.
   */
  void UpdateState() {
    // Update acceleration of each particle
    for (size_t i = 0; i < n; ++i) {
      T inv_mi = T(1.) / m[i];
      ax[i] = Fx[i] * inv_mi;
      ay[i] = Fy[i] * inv_mi;
      az[i] = Fz[i] * inv_mi;
    }
    for (size_t i = 0; i < n; ++i) {
      vx[i] += ax[i] * d_t;
      vy[i] += ay[i] * d_t;
      vz[i] += az[i] * d_t;

      x[i] += vx[i] * d_t;
      y[i] += vy[i] * d_t;
      z[i] += vz[i] * d_t;
    }
  }

  // Temporary for testing purposes
  // TODO: parametrize. Maybe not a very bad function to have, after all.
  void Randomize() {
    RNG<T> rng;
    rng.GenerateUniformRandom(x, n, -2., 2.);
    rng.GenerateUniformRandom(y, n, -2., 2.);
    rng.GenerateUniformRandom(z, n, -2., 2.);

    rng.GenerateUniformRandom(m, n, 1., 200.);

    rng.GenerateUniformRandom(vx, n, -20., 20.);
    rng.GenerateUniformRandom(vy, n, -20., 20.);
    rng.GenerateUniformRandom(vz, n, -20., 20.);
  }

 public:
  // TODO: Possibly useless after debug. Clean-up
  void Print(const size_t begin, const size_t end) const {
    for (size_t i = begin; i <= end; ++i) {
      // unfortunately current implementation of std::println is slow
      // TODO: change back to println when issues fixed.
      // std::println(
      //     "particle: {}--> x: {}, y: {}, z: {}, m: {}, vx: {}, vy: {}, vz:
      //     {}", i, x[i], y[i], z[i], m[i], vx[i], vy[i], vz[i]);
      // std::println("    Fx: {}, Fy: {}, Fz: {}, ax: {}, ay: {}, az: {}",
      // Fx[i],
      //              Fy[i], Fz[i], ax[i], ay[i], az[i]);
      // std::println("============================");
      // std::printf("i: %x x: %.2f", i, x[i]);
      std::printf(
          "particle %zd\tp(%.2f,%.2f, %.2f)\tm: %.2f\tv(%.2f, %.2f,%.2f)\n", i,
          x[i], y[i], z[i], m[i], vx[i], vy[i], vz[i]);
      std::printf("\t\ta(%.2f,%.2f, %.2f)\tF(%.2f, %.2f,%.2f)\n", ax[i], ay[i],
                  az[i], Fx[i], Fy[i], Fz[i]);
    }
  }
};
