#include <catch2/catch.hpp>

#include "../serial_test_util.hpp"
#include "beyond/math/point.hpp"
#include "beyond/math/serial.hpp"

TEST_CASE("Points", "[beyond.core.math.vec]")
{
  SECTION("Create 3d TPoint from 2d TPoint and a scalar")
  {
    const beyond::Point2 p1{1, 2};
    const beyond::Point3 p2{p1, 3};
  }

  const beyond::Point3 p1{1, 2.4f, 3};
  const beyond::Point3 p2{4, 5.4f, 4};
  const beyond::Vec3 v1{2, 3.3f, 4.2f};

  SECTION("TPoint TVec addition")
  {
    SECTION("TPoint += TVec")
    {
      beyond::Point3 result1 = p1;
      result1 += v1;
      REQUIRE(result1.x == Approx(p1.x + v1.x));
      REQUIRE(result1.y == Approx(p1.y + v1.y));
      REQUIRE(result1.z == Approx(p1.z + v1.z));
    }

    SECTION("TPoint + TVec = TPoint")
    {
      const beyond::Point3 result1 = p1 + v1;
      REQUIRE(result1.x == Approx(p1.x + v1.x));
      REQUIRE(result1.y == Approx(p1.y + v1.y));
      REQUIRE(result1.z == Approx(p1.z + v1.z));
    }

    SECTION("TVec + TPoint = TPoint")
    {
      const beyond::Point3 result1 = v1 + p1;
      REQUIRE(result1.x == Approx(p1.x + v1.x));
      REQUIRE(result1.y == Approx(p1.y + v1.y));
      REQUIRE(result1.z == Approx(p1.z + v1.z));
    }
  }

  SECTION("TPoint TVec subtraction")
  {
    SECTION("TPoint -= TVec")
    {
      beyond::Point3 result1 = p1;
      result1 -= v1;
      REQUIRE(result1.x == Approx(p1.x - v1.x));
      REQUIRE(result1.y == Approx(p1.y - v1.y));
      REQUIRE(result1.z == Approx(p1.z - v1.z));
    }

    SECTION("TPoint - TVec = TPoint")
    {
      const beyond::Point3 result1 = p1 - v1;
      REQUIRE(result1.x == Approx(p1.x - v1.x));
      REQUIRE(result1.y == Approx(p1.y - v1.y));
      REQUIRE(result1.z == Approx(p1.z - v1.z));
    }
  }

  SECTION("Distance between two points")
  {
    const auto dx = p2 - p1;
    const auto dist2 = distance_squared(p1, p2);
    const auto dist = distance(p1, p2);

    REQUIRE(dist2 == Approx(dot(dx, dx)));
    REQUIRE(dist == Approx(sqrt(dist2)));
  }
}

TEST_CASE("Point Swizzling test", "[beyond.core.math.pec]")
{
  constexpr float a = 2.1f;
  constexpr float b = 4.2f;
  constexpr float c = 6.3f;

  beyond::Point2 p1{a, b};
  beyond::Point3 p2{a, b, c};

  SECTION("Equality test")
  {
    const beyond::Point2 p4 = p1.xy;
    CHECK(p1 == p4);
    CHECK(p1 == p1.xy);
    CHECK(p1.xy == p1.xy);

    const beyond::Point2 p5 = p1.yx;
    CHECK(p1 != p5);
    CHECK(p1 != p5.xy);
    CHECK(p1.xy != p1.yx);
  }

  SECTION("Swizzle assignment")
  {
    p2 = p2.yxz;
    CHECK(p2.x == Approx(b));
    CHECK(p2.y == Approx(a));
    CHECK(p2.z == Approx(c));
  }

  SECTION("Arithmetics on swizzed structures")
  {
    SECTION("Scalar multiplication")
    {
      const beyond::Point2 result1 = p1.xy * 2.f;
      const beyond::Point2 result2 = 2.f * p1.xy;
      CHECK(result1.x == Approx(p1.x * 2.f));
      CHECK(result1.y == Approx(p1.y * 2.f));
      CHECK(result1 == result2);

      p1.xy *= 2.f;
      CHECK(result1 == p1);
    }

    SECTION("Scalar dipision")
    {
      const beyond::Point2 result1 = p1.xy / 2.f;
      CHECK(result1.x == Approx(p1.x / 2.f));
      CHECK(result1.y == Approx(p1.y / 2.f));

      p1.xy /= 2.f;
      CHECK(result1 == p1);
    }

    SECTION("Addition")
    {
      const beyond::Point2 result1 = p1.xy + p1.yx;
      SECTION("Binary additions")
      {
        CHECK(result1.x == Approx(p1.x + p1.y));
        CHECK(result1.y == Approx(p1.x + p1.y));

        const auto result2 = p1.xy + p1;
        CHECK(result2.x == Approx(p1.x * 2.f));
        CHECK(result2.y == Approx(p1.y * 2.f));

        const auto result3 = p1 + p1.xy;
        CHECK(result3 == result2);
      }

      SECTION("Self increment")
      {
        p1.yx += p1;
        REQUIRE(p1 == result1);
      }

      SECTION("Self increment with another swizzler")
      {
        p1.yx += p1.xy;
        REQUIRE(p1 == result1);
      }
    }

    SECTION("Subtraction")
    {
      const beyond::Point2 result1 = p1.xy - p1.yx;

      SECTION("Binary additions")
      {
        CHECK(result1.x == Approx(p1.x - p1.y));
        CHECK(result1.y == Approx(p1.y - p1.x));

        const auto result2 = p1.xy - p1;
        CHECK(result2.x == Approx(0));
        CHECK(result2.y == Approx(0));

        const auto result3 = p1 - p1.xy;
        CHECK(result3 == result2);
      }

      SECTION("Self decrement")
      {
        p1.yx -= p1;
        REQUIRE(p1 == result1);
      }

      SECTION("Self decrement with another swizzler")
      {
        p1.yx -= p1.xy;
        REQUIRE(p1 == result1);
      }
    }

    SECTION("dot product")
    {
      const beyond::Point2 p4{b, a};
      const auto result = 2 * a * b;

      CHECK(dot(p1.xy, p4.xy) == Approx(result));

      CHECK(dot(p1.xy, p4) == Approx(result));

      CHECK(dot(p1, p4.xy) == Approx(result));
    }
  }
}

TEST_CASE("Point serialization test", "[beyond.core.math.vec]")
{
  SECTION("Output TPoint2 to a stream")
  {
    const beyond::Point2 p{1, 2};
    const auto expected = "point(1, 2)";

    REQUIRE_TO_STRING_EQ(p, expected);
  }

  SECTION("Output TPoint3 to a stream")
  {
    const beyond::Point3 p{1, 2, 3};
    const auto expected = "point(1, 2, 3)";

    REQUIRE_TO_STRING_EQ(p, expected);
  }
}
