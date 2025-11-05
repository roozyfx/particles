#pragma once
#include <cmath>
#include <concepts>
#include <memory>
#include <ostream>

#include "types.hpp"

template <typename T>
class Vector {
 public:
  T x{}, y{}, z{};

  Vector() = default;
  Vector(const T& x, const T& y, const T& z) : x{x}, y{y}, z{z} {}
  Vector(const Vector& other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  Vector& operator=(const Vector& rhs) {
    if (&rhs == this) {
      return *this;
    }
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;

    return *this;
  }

  bool operator==(const Vector&) = delete;

  bool IsAlmostEqual(
      const Vector& rhs,
      T epsilon = std::numeric_limits<T>::epsilon()) const noexcept {
    return std::abs(x - rhs.x) < epsilon && std::abs(y - rhs.y) < epsilon &&
           std::abs(z - rhs.z) < epsilon;
  }

  inline Vector& operator+=(const Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  friend Vector operator+(Vector v1, const Vector& v2) {
    v1 += v2;
    return v1;
  }

  inline Vector& operator-=(const Vector& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  friend Vector operator-(Vector v1, const Vector& v2) {
    v1 -= v2;
    return v1;
  }

  inline Vector& operator*=(const T k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
  }

  friend Vector operator*(const DType& k, const Vector& v) {
    Vector r;
    r.x = v.x * k;
    r.y = v.y * k;
    r.z = v.z * k;
    return r;
  }

  friend Vector operator*(const Vector& v, const DType& k) {
    return operator*(k, v);
  }

  friend Vector operator/(const Vector& v, const DType& k) {
    return operator*(1. / k, v);
  }

  inline Vector& operator/=(const T k) {
    x /= k;
    y /= k;
    z /= k;
    return *this;
  }

  inline T operator[](const size_t i) const {
    if (i == 0) return x;
    if (i == 1) return y;
    if (i == 2) return z;
    throw std::logic_error("invalid index");
  }

  inline T& operator[](const size_t i) {
    if (i == 0) return x;
    if (i == 1) return y;
    if (i == 2) return z;
    throw std::logic_error("invalid index");
  }

  friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << v.x << ", " << v.y << ", " << v.z;
    return os;
  }

  // L2 norm
  inline constexpr T Norm() {
    using std::sqrt;
    return sqrt(x * x + y * y + z * z);
  }

  // (L2 norm)²
  inline constexpr T Norm2() { return (x * x + y * y + z * z); }
};
#include <stdfloat>
// using Vec3f = Vector<std::float32_t>;
using Vec3d = Vector<double>;
using Vec3i = Vector<IType>;
