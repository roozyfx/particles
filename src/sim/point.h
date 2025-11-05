#pragma once
#include <limits>
#include <stdfloat>

#include "vectors.h"

// requires std::floating_point<T>
template <typename T>
struct Point {
  T x, y, z;
  Point(const T x, T y, T z) : x{x}, y{y}, z{z} {}

  Point(const Point& other) = default;

  Point& operator=(const Point& p) {
    if (&p == this) {
      return *this;
    }
    x = p.x;
    y = p.y;
    z = p.z;

    return *this;
  }

  // inline constexpr bool operator==(const Point& p) const {
  //   return x == p.x && y == p.y && z == p.z;
  // }

  bool operator==(const Point&) = delete;
  bool constexpr IsAlmostEqual(
      const Point& rhs,
      T epsilon = std::numeric_limits<T>::epsilon()) const noexcept {
    return std::abs(x - rhs.x) < epsilon && std::abs(y - rhs.y) < epsilon &&
           std::abs(z - rhs.z) < epsilon;
  }

  Point& operator+=(const Vector<T>& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  friend Point operator+(Point p, const Vector<T>& v) {
    p += v;
    return p;
  }

  friend Point operator+(const Vector<T>& v, Point p) {
    return operator+(p, v);
  }

  Point& operator-=(const Vector<T>& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  friend Point operator-(Point p, const Vector<T>& v) {
    p -= v;
    return p;
  }

  // friend Point operator-(const Vector<T>& v, const Point& p) {
  //   return operator-(p, v);
  // }

  friend Vector<T> operator-(const Point& p2, const Point& p1) {
    return Vector<T>(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
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

  friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ", " << p.y << ", " << p.z;
    return os;
  }
};

using Point3d = Point<double>;
// using Point3f = Point<std::float32_t>;
