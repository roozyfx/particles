#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "sim/linear_algebra.h"
#include "sim/types.hpp"

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

  Point3d q(-1, -5, -10);
  Point3d qq = q + v;
  EXPECT_TRUE(qq.IsAlmostEqual(Point3d(2, -1, -5)));

  Point3d start(4, 5, 6);
  Point3d end(2, 0, -1);
  EXPECT_TRUE((end - start).IsAlmostEqual(Vec3d(-2, -5, -7)));
}

TEST(VectorTest, BasicOperations) {
  Vec3d x(1, 0, 0);
  Vec3d y(0, 1, 0);
  Vec3d z(0, 0, 1);
  Vec3d xy(1., 1., 0.);
  Vec3d xz(1, 0, 1);
  Vec3d yz(0, 1, 1);
  EXPECT_TRUE((x + y).IsAlmostEqual(xy));
  EXPECT_TRUE((x + z).IsAlmostEqual(xz));
  EXPECT_TRUE((z + y).IsAlmostEqual(yz));

  EXPECT_EQ(x.Norm2(), 1);
  EXPECT_EQ(y.Norm2(), 1);
  EXPECT_EQ(z.Norm2(), 1);
  EXPECT_EQ(xz.Norm2(), 2);
  EXPECT_EQ(xy.Norm(), sqrt(2));
  Vec3d v(4, 3, 2);
  Vec3d u(2, 3, 4);

  Vec3d vcopy(v);
  Vec3d vOpAssign(v);
  EXPECT_TRUE(vcopy.IsAlmostEqual(v));
  EXPECT_TRUE(vOpAssign.IsAlmostEqual(vcopy));

  EXPECT_TRUE((v + u).IsAlmostEqual(Vec3d(6, 6, 6)));
  v += u;
  EXPECT_TRUE(v.IsAlmostEqual(Vec3d(6, 6, 6)));

  Vec3d vv(4, 3, 2);
  DType k = 3.5f;
  EXPECT_TRUE((k * vv).IsAlmostEqual(vv * k));
  EXPECT_TRUE((k * vv).IsAlmostEqual(Vec3d(14, 10.5, 7)));
  vv *= k;
  EXPECT_TRUE(vv.IsAlmostEqual(Vec3d(14, 10.5, 7)));
  vv = vv / k;
  EXPECT_TRUE(vv.IsAlmostEqual(Vec3d(4, 3, 2)));
  EXPECT_EQ(vv[0], 4);
  EXPECT_EQ(vv[1], 3);
  EXPECT_EQ(vv[2], 2);
  std::stringstream ss;
  ss << vv;
  std::string str = ss.str();
  EXPECT_STREQ(str.c_str(), "4, 3, 2");

  EXPECT_EQ(vv.Norm2(), 16 + 9 + 4);
  Vec3d w(0, 3, 4);
  EXPECT_EQ(w.Norm(), 5);
}
