#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "../src/sim/types.hpp"
#include "../src/sim/vectors.h"

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
  EXPECT_TRUE((v - v).IsAlmostEqual(Vec3d(0, 0, 0)));
  EXPECT_TRUE((v - u).IsAlmostEqual(Vec3d(2, 0, -2)));
  v += u;
  EXPECT_TRUE(v.IsAlmostEqual(Vec3d(6, 6, 6)));
  v -= u;
  EXPECT_TRUE(v.IsAlmostEqual(Vec3d(4, 3, 2)));
  DType k = 3.5f;
  EXPECT_TRUE((k * v).IsAlmostEqual(v * k));
  EXPECT_TRUE((k * v).IsAlmostEqual(Vec3d(14, 10.5, 7)));
  v *= k;
  EXPECT_TRUE(v.IsAlmostEqual(Vec3d(14, 10.5, 7)));
  v = v / k;
  EXPECT_TRUE(v.IsAlmostEqual(Vec3d(4, 3, 2)));
  EXPECT_EQ(v[0], 4);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(v[2], 2);
  std::stringstream ss;
  ss << v;
  std::string str = ss.str();
  EXPECT_STREQ(str.c_str(), "4, 3, 2");

  EXPECT_EQ(v.Norm2(), 16 + 9 + 4);
  Vec3d w(0, 3, 4);
  EXPECT_EQ(w.Norm(), 5);
}
