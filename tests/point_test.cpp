#include "../src/sim/point.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "../src/sim/types.hpp"
#include "../src/sim/vectors.h"

TEST(PointTest, BasicOpeartionAndMethods) {
  Point3d p(1, 2, 3);
  Point3d pcopy(p);
  Point3d pOpAsgn = p;
  EXPECT_TRUE(p.IsAlmostEqual(pcopy));
  EXPECT_TRUE(p.IsAlmostEqual(pOpAsgn));
  EXPECT_TRUE(p.IsAlmostEqual(p));

  Vec3d v(3, 4, 5);
  p += v;
  EXPECT_TRUE(p.IsAlmostEqual(Point3d(4, 6, 8)));
  p -= v;
  EXPECT_TRUE(p.IsAlmostEqual(pcopy));

  Point3d q(-1, -5, -10);
  Point3d qq = q + v;
  EXPECT_TRUE(qq.IsAlmostEqual(Point3d(2, -1, -5)));

  Point3d start(4, 5, 6);
  Point3d end(2, 0, -1);
  EXPECT_TRUE((end - start).IsAlmostEqual(Vec3d(-2, -5, -7)));
}