#pragma once

#include <array>
#include <random>
#include <type_traits>
#include <vector>

using std::uniform_real_distribution;
using std::vector;

template <typename T>
class RNG {
 public:
  /**
   * @brief Returns std::vector of the type T with values uniformly distributed
   * between low and high. In the case of T a compound-type, low and high will
   * define the boundries with their corresponding index: low[0]-high[0] for the
   * 0th
   * @param data : the raw array
   */
  vector<T> GenerateUniformRandom(const size_t n, const T& low, const T& high);

  /**
   * @brief Populates a raw array of size n with uniformly distributed values
   * between low and high
   * @param data : the raw array
   */
  void GenerateUniformRandom(T* data, const size_t n, const T& low,
                             const T& high);
};

#include "rng.cpp"