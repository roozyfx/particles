#pragma once

#include <cmath>
#include <concepts>
#include <cstdio>
#include <iostream>
#include <vector>
#ifdef PARALLEL
#include <execution>
#define PAR std::execution::par,
#else
#define PAR
#endif

#include "../utils/rng.h"
#include "constants.hpp"

/**
 * A Particle System, as a Struct of Arrays of properties
 */
template <std::floating_point T = double>
class Particles {
  // number of particles
  size_t n;
  T d_t;
  using VecT = std::vector<T>;
  // Positions
  VecT x, y, z;
  // Masses
  VecT m;
  VecT Gm;
  // velocity
  VecT vx, vy, vz;

  // Force on each particle. This is temporary, if I choose Gravity between
  // particles as a the simulated force.
  VecT Fx, Fy, Fz;
  static T constexpr epsilon{T(0.0001)};
  static T constexpr eps2{epsilon * epsilon};

 public:
  Particles(const size_t n, const T d_t)
      : n{n},
        d_t{d_t},
        x{VecT(n)},
        y{VecT(n)},
        z{VecT(n)},
        m{VecT(n)},
        Gm{VecT(n)},
        vx{VecT(n)},
        vy{VecT(n)},
        vz{VecT(n)},
        Fx{VecT(n)},
        Fy{VecT(n)},
        Fz{VecT(n)} {
    Randomize();
    std::transform(std::begin(m), std::end(m), std::begin(Gm),
                   [](auto mi) { return G * mi; });
  }

  Particles(const Particles&) = delete;
  Particles& operator=(const Particles&) = delete;
  Particles(Particles&&) noexcept = default;
  Particles& operator=(Particles&&) noexcept = default;
  ~Particles() = default;
  /**
   * @brief Updates this particle's velocity and position, using a semi-Euler
   * method
   *
   */
  // @todo: Make different options for Force, F. such as gravity between
  // particles, no inner-force just initial velocity and global (earth) gravity,
  // etc.
  void Update(const VecT& F_ext_x, const VecT& F_ext_y, const VecT& F_ext_z,
              const T& F_global_x, const T& F_global_y, const T& F_global_z) {
    UpdateForces(F_ext_x, F_ext_y, F_ext_z, F_global_x, F_global_y, F_global_z);

    UpdateState();
  }

 private:
  void UpdateForces(const VecT& F_ext_x, const VecT& F_ext_y,
                    const VecT& F_ext_z, const T& F_global_x,
                    const T& F_global_y, const T& F_global_z) {
    std::fill(PAR begin(Fx), end(Fx), T(0));
    std::fill(PAR begin(Fy), end(Fy), T(0));
    std::fill(PAR begin(Fz), end(Fz), T(0));

    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        const T rx{x[j] - x[i]};
        const T ry{y[j] - y[i]};
        const T rz{z[j] - z[i]};

        // eps² for smoothing
        const T r2{rx * rx + ry * ry + rz * rz + eps2};
        using std::sqrt;
        const T inv_r{T(1.) / sqrt(r2)};
        T inv_r3 = inv_r * inv_r * inv_r;

        T F = Gm[i] * m[j] * inv_r3;
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
    if (!F_ext_x.empty())
      std::transform(PAR std::begin(Fx), std::end(Fx), std::begin(F_ext_x),
                     std::begin(Fx), std::plus<>{});
    if (!F_ext_y.empty())
      std::transform(PAR std::begin(Fy), std::end(Fy), std::begin(F_ext_y),
                     std::begin(Fy), std::plus<>{});
    if (!F_ext_z.empty())
      std::transform(PAR std::begin(Fz), std::end(Fz), std::begin(F_ext_z),
                     std::begin(Fz), std::plus<>{});
    // Add global force
    std::for_each(PAR std::begin(Fx), std::end(Fx),
                  [f_g = F_global_x](auto& f) { f += f_g; });
    std::for_each(PAR std::begin(Fy), std::end(Fy),
                  [f_g = F_global_y](auto& f) { f += f_g; });
    std::for_each(PAR std::begin(Fz), std::end(Fz),
                  [f_g = F_global_z](auto& f) { f += f_g; });
  }

  /**
   * @brief Semi-Euler update on this particle's velocity and position.
   */
  void UpdateState() {
    using std::begin;
    using std::end;

    // velocity += F/m * d_t
    // x
    std::transform(PAR begin(Fx), end(Fx), begin(m), begin(Fx),
                   [dt = d_t](const T& f, const T& m) { return f * dt / m; });
    std::transform(PAR begin(Fx), end(Fx), begin(vx), begin(vx),
                   [](const T& f, const T& v) { return f + v; });
    // y
    std::transform(PAR begin(Fy), end(Fy), begin(m), begin(Fy),
                   [dt = d_t](const T& f, const T& m) { return f * dt / m; });
    std::transform(PAR begin(Fy), end(Fy), begin(vy), begin(vy),
                   [](const T& f, const T& v) { return f + v; });
    // z
    std::transform(PAR begin(Fz), end(Fz), begin(m), begin(Fz),
                   [dt = d_t](const T& f, const T& m) { return f * dt / m; });
    std::transform(PAR begin(Fz), end(Fz), begin(vz), begin(vz),
                   [](const T& f, const T& v) { return f + v; });

    // position += velocity * d_t
    std::transform(PAR begin(vx), end(vx), begin(x), begin(x),
                   [dt = d_t](const T& v, T& p) { return std::fma(v, dt, p); });
    std::transform(PAR begin(vy), end(vy), begin(y), begin(y),
                   [dt = d_t](const T& v, T& p) { return std::fma(v, dt, p); });
    std::transform(PAR begin(vz), end(vz), begin(z), begin(z),
                   [dt = d_t](const T& v, T& p) { return std::fma(v, dt, p); });
  }

  // Temporary for testing purposes
  // TODO: parametrize. Maybe not a very bad function to have, after all.
  void Randomize() {
    RNG<T> rng;
    rng.GenerateUniformRandom(x.data(), n, -2., 2.);
    rng.GenerateUniformRandom(y.data(), n, -2., 2.);
    rng.GenerateUniformRandom(z.data(), n, -2., 2.);

    rng.GenerateUniformRandom(m.data(), n, 1., 200.);

    rng.GenerateUniformRandom(vx.data(), n, -20., 20.);
    rng.GenerateUniformRandom(vy.data(), n, -20., 20.);
    rng.GenerateUniformRandom(vz.data(), n, -20., 20.);
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
      std::printf("\tF(%.2f, %.2f,%.2f)\n", Fx[i], Fy[i], Fz[i]);
    }
  }
};
