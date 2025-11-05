#include "../src/particle.cpp"

#include <gtest/gtest.h>

// #include <sstream>
// #include <string>

TEST(ParticleTest, RationalityTest) {
  Point3d pos0(0, 0, 0);
  DType m0{1};
  Vec3d v0{0, 0, 0};
  Particle p0(pos0, m0, v0, 0);

  // Vec3d a(0., 0., 0.);
  // p0.UpdatePosition(a, 1);
  // EXPECT_EQ(p0.p.IsAlmostEqual(pos0), true);
  // p0.v = Vec3d(1., 0, 0);
  // p0.UpdatePosition(a, 1);
  // EXPECT_EQ(p0.p.IsAlmostEqual(pos0 + Vec3d(1, 0, 0)), true);
  // a = Vec3d(1., 0., 0.);
  // p0.UpdatePosition(a, 1);
  // EXPECT_EQ(p0.p.IsAlmostEqual(pos0 + Vec3d(2.5, 0, 0)), true);
}
