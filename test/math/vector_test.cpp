#include <catch2/catch.hpp>

#include <sstream>

#include "../serial_test_util.hpp"
#include "beyond/core/math/serial.hpp"
#include "beyond/core/math/vector.hpp"

TEST_CASE("All Vector types test", "[beyond.core.math.vec]")
{
  using Catch::literals::operator""_a;

  SECTION("Default construction 0-initialize vectors")
  {
    SECTION("2d Vectors")
    {
      beyond::Vector2i v;
      REQUIRE(v.x == 0);
      REQUIRE(v.y == 0);
    }

    SECTION("3d Vectors")
    {
      beyond::Vector3i v;
      REQUIRE(v.x == 0);
      REQUIRE(v.y == 0);
      REQUIRE(v.z == 0);
    }

    SECTION("4d Vectors")
    {
      beyond::Vector4i v;
      REQUIRE(v.x == 0);
      REQUIRE(v.y == 0);
      REQUIRE(v.z == 0);
      REQUIRE(v.w == 0);
    }
  }

  const int x1 = 1;
  const int y1 = 2;
  const int z1 = 3;
  const int w1 = 4;
  const int x2 = 1;
  const int y2 = 2;
  const int z2 = 3;
  const int w2 = 4;

  SECTION("Construct higher dimensional vectors from lower dimension")
  {
    GIVEN("A 2d Vector v1 = [x1, y1]")
    {
      const beyond::Vector2i v1{x1, y1};

      AND_GIVEN("A 3d vector v2 created from v1 and z1")
      {
        const beyond::Vector3i v2{v1, z1};

        THEN("v2 = [v1, z1]")
        {
          REQUIRE(v2.x == v1.x);
          REQUIRE(v2.y == v1.y);
          REQUIRE(v2.z == z1);

          WHEN("Create a 4d vector v3 from v2 and w1")
          {

            const beyond::Vector4i v3{v2, w1};

            THEN("v3 = [v1, z1, w1]")
            {
              REQUIRE(v3.x == v1.x);
              REQUIRE(v3.y == v1.y);
              REQUIRE(v3.z == z1);
              REQUIRE(v3.w == w1);
            }
          }
        }
      }

      WHEN("Create a 4d vector from v1, z1, and w1")
      {
        const beyond::Vector4i v2{v1, z1, w1};
        THEN("v2 = [v1, z1, w1]")
        {
          REQUIRE(v2.x == v1.x);
          REQUIRE(v2.y == v1.y);
          REQUIRE(v2.z == z1);
          REQUIRE(v2.w == w1);
        }
      }
    }
  }

  SECTION("Negating")
  {
    const beyond::Vector4i v1{x1, y1, z1, w1};
    const beyond::Vector4i result = -v1;
    REQUIRE(result.x == -x1);
    REQUIRE(result.y == -y1);
    REQUIRE(result.z == -z1);
    REQUIRE(result.w == -w1);
  }

  SECTION("operator+")
  {
    beyond::Vector4i v1{x1, y1, z1, w1};
    beyond::Vector4i v2{x2, y2, z2, w2};
    const beyond::Vector4i result = v1 + v2;
    REQUIRE(result.x == (v1.x + v2.x));
    REQUIRE(result.y == (v1.y + v2.y));
    REQUIRE(result.z == (v1.z + v2.z));
    REQUIRE(result.w == (v1.w + v2.w));
  }

  SECTION("operator+=")
  {
    beyond::Vector4i v1{x1, y1, z1, w1};
    beyond::Vector4i v2{x2, y2, z2, w2};
    beyond::Vector4i result = v1;
    result += v2;
    REQUIRE(result.x == (v1.x + v2.x));
    REQUIRE(result.y == (v1.y + v2.y));
    REQUIRE(result.z == (v1.z + v2.z));
    REQUIRE(result.w == (v1.w + v2.w));
  }

  SECTION("operator-")
  {
    const beyond::Vector4i v1{x1, y1, z1, w1};
    const beyond::Vector4i v2{x2, y2, z2, w2};
    const beyond::Vector4i result = v1 - v2;
    REQUIRE(result.x == (v1.x - v2.x));
    REQUIRE(result.y == (v1.y - v2.y));
    REQUIRE(result.z == (v1.z - v2.z));
    REQUIRE(result.w == (v1.w - v2.w));
  }

  SECTION("operator-=")
  {
    const beyond::Vector4i v1{x1, y1, z1, w1};
    const beyond::Vector4i v2{x2, y2, z2, w2};
    beyond::Vector4i result = v1;
    result -= v2;
    REQUIRE(result.x == (v1.x - v2.x));
    REQUIRE(result.y == (v1.y - v2.y));
    REQUIRE(result.z == (v1.z - v2.z));
    REQUIRE(result.w == (v1.w - v2.w));
  }

  SECTION("operator*=")
  {
    const beyond::Vector4i v1{x1, y1, z1, w1};
    const int v2 = x2;
    beyond::Vector4i result = v1;
    result *= v2;
    REQUIRE(result.x == (v1.x * v2));
    REQUIRE(result.y == (v1.y * v2));
    REQUIRE(result.z == (v1.z * v2));
    REQUIRE(result.w == (v1.w * v2));
  }

  SECTION("Equality tests")
  {
    const beyond::Vector4i v1{x1, y1, z1, w1};
    const beyond::Vector4i v2{x1, y1, z1, w1};
    const beyond::Vector4i v3{x1 + 1, y1, z1, w1};
    const beyond::Vector4i v4{x1, y1 + 1, z1, w1};
    const beyond::Vector4i v5{x1, y1, z1 + 1, w1};
    const beyond::Vector4i v6{x1, y1, z1, w1 + 1};
    REQUIRE(v1 == v2);
    REQUIRE(v2 != v3);
    REQUIRE(v2 != v4);
    REQUIRE(v2 != v5);
    REQUIRE(v2 != v6);
  }

  SECTION("Dot product")
  {
    const beyond::Vector4i v3{x1, y1, z1, w1};
    REQUIRE(beyond::dot(v3, v3) == (x1 * x1 + y1 * y1 + z1 * z1 + w1 * w1));
  }

  SECTION("Index accessing by [] and ()")
  {
    const beyond::Vector4i v1{x1, y1, z1, w1};
    REQUIRE(v1[0] == x1);
    REQUIRE(v1(3) == w1);
  }
}

TEST_CASE("Floating point Vector only test", "[beyond.core.math.vec]")
{
  using Catch::literals::operator""_a;

  const auto x1 = 1.2f;
  const auto y1 = 2.4f;
  const auto z1 = 3.6f;
  const auto w1 = 4.8f;
  const auto x2 = 1.1f;

  SECTION("operator/")
  {
    const beyond::Vector4f v1{x1, y1, z1, w1};
    const beyond::Vector4f result = v1 / x2;
    REQUIRE(result.x == Approx(v1.x / x2));
    REQUIRE(result.y == Approx(v1.y / x2));
    REQUIRE(result.z == Approx(v1.z / x2));
    REQUIRE(result.w == Approx(v1.w / x2));
  }

  SECTION("operator/=")
  {
    const beyond::Vector4f v1{x1, y1, z1, w1};
    beyond::Vector4f result = v1;
    result /= x2;
    REQUIRE(result.x == Approx(v1.x / x2));
    REQUIRE(result.y == Approx(v1.y / x2));
    REQUIRE(result.z == Approx(v1.z / x2));
    REQUIRE(result.w == Approx(v1.w / x2));
  }

  SECTION("normalize")
  {
    const beyond::Vector4f v1{x1, y1, z1, w1};
    const beyond::Vector4f result = beyond::normalize(v1);
    REQUIRE(result.x == Approx(v1.x / v1.length()));
    REQUIRE(result.y == Approx(v1.y / v1.length()));
    REQUIRE(result.z == Approx(v1.z / v1.length()));
    REQUIRE(result.w == Approx(v1.w / v1.length()));
  }
}

TEST_CASE("Points", "[beyond.core.math.vec]")
{
  SECTION("Create 3d Point from 2d Point and a scalar")
  {
    const beyond::Point2f p1{1, 2};
    const beyond::Point3f p2{p1, 3};
  }

  const beyond::Point3f p1{1, 2.4f, 3};
  const beyond::Point3f p2{4, 5.4f, 4};
  const beyond::Vector3f v1{2, 3.3f, 4.2f};

  SECTION("Point Vector addition")
  {
    SECTION("Point += Vector")
    {
      beyond::Point3f result1 = p1;
      result1 += v1;
      REQUIRE(result1.x == Approx(p1.x + v1.x));
      REQUIRE(result1.y == Approx(p1.y + v1.y));
      REQUIRE(result1.z == Approx(p1.z + v1.z));
    }

    SECTION("Point + Vector = Point")
    {
      const beyond::Point3f result1 = p1 + v1;
      REQUIRE(result1.x == Approx(p1.x + v1.x));
      REQUIRE(result1.y == Approx(p1.y + v1.y));
      REQUIRE(result1.z == Approx(p1.z + v1.z));
    }

    SECTION("Vector + Point = Point")
    {
      const beyond::Point3f result1 = v1 + p1;
      REQUIRE(result1.x == Approx(p1.x + v1.x));
      REQUIRE(result1.y == Approx(p1.y + v1.y));
      REQUIRE(result1.z == Approx(p1.z + v1.z));
    }
  }

  SECTION("Point Vector subtraction")
  {
    SECTION("Point -= Vector")
    {
      beyond::Point3f result1 = p1;
      result1 -= v1;
      REQUIRE(result1.x == Approx(p1.x - v1.x));
      REQUIRE(result1.y == Approx(p1.y - v1.y));
      REQUIRE(result1.z == Approx(p1.z - v1.z));
    }

    SECTION("Point - Vector = Point")
    {
      const beyond::Point3f result1 = p1 - v1;
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

TEST_CASE("Vector Swizzling", "[beyond.core.math.vec]")
{
  constexpr float a = 2.1f;
  constexpr float b = 4.2f;
  constexpr float c = 6.3f;
  constexpr float d = 8.4f;

  beyond::Vector2f v1{a, b};
  beyond::Vector3f v2{a, b, c};
  beyond::Vector4f v3{a, b, c, d};

  SECTION("Equality test")
  {
    const beyond::Vector2f v4 = v1.xy;
    CHECK(v1 == v4);
    CHECK(v1 == v1.xy);
    CHECK(v1.xy == v1.xy);

    const beyond::Vector2f v5 = v1.yx;
    CHECK(v1 != v5);
    CHECK(v1 != v5.xy);
    CHECK(v1.xy != v1.yx);
  }

  SECTION("Swizzle assignment")
  {
    v1.yx = v1;
    CHECK(v1.x == Approx(b));
    CHECK(v1.y == Approx(a));

    v1.yx = v1.xy;
    CHECK(v1.x == Approx(a));
    CHECK(v1.y == Approx(b));

    v2 = v2.yxz;
    CHECK(v2.x == Approx(b));
    CHECK(v2.y == Approx(a));
    CHECK(v2.z == Approx(c));

    v3 = v3.wzyx;
    CHECK(v3.x == Approx(d));
    CHECK(v3.y == Approx(c));
    CHECK(v3.z == Approx(b));
    CHECK(v3.w == Approx(a));
  }

  SECTION("Arithmetics on swizzed structures")
  {
    SECTION("Scalar multiplication")
    {
      const beyond::Vector2f result1 = v1.xy * 2.f;
      const beyond::Vector2f result2 = 2.f * v1.xy;
      CHECK(result1.x == Approx(v1.x * 2.f));
      CHECK(result1.y == Approx(v1.y * 2.f));
      CHECK(result1 == result2);

      v1.xy *= 2.f;
      CHECK(result1 == v1);
    }

    SECTION("Scalar division")
    {
      const beyond::Vector2f result1 = v1.xy / 2.f;
      CHECK(result1.x == Approx(v1.x / 2.f));
      CHECK(result1.y == Approx(v1.y / 2.f));

      v1.xy /= 2.f;
      CHECK(result1 == v1);
    }

    SECTION("Addition")
    {
      const beyond::Vector2f result1 = v1.xy + v1.yx;
      SECTION("Binary additions")
      {
        CHECK(result1.x == Approx(v1.x + v1.y));
        CHECK(result1.y == Approx(v1.x + v1.y));

        const auto result2 = v1.xy + v1;
        CHECK(result2.x == Approx(v1.x * 2.f));
        CHECK(result2.y == Approx(v1.y * 2.f));

        const auto result3 = v1 + v1.xy;
        CHECK(result3 == result2);
      }

      SECTION("Self increment")
      {
        v1.yx += v1;
        REQUIRE(v1 == result1);
      }

      SECTION("Self increment with another swizzler")
      {
        v1.yx += v1.xy;
        REQUIRE(v1 == result1);
      }
    }

    SECTION("Subtraction")
    {
      const beyond::Vector2f result1 = v1.xy - v1.yx;

      SECTION("Binary additions")
      {
        CHECK(result1.x == Approx(v1.x - v1.y));
        CHECK(result1.y == Approx(v1.y - v1.x));

        const auto result2 = v1.xy - v1;
        CHECK(result2.x == Approx(0));
        CHECK(result2.y == Approx(0));

        const auto result3 = v1 - v1.xy;
        CHECK(result3 == result2);
      }

      SECTION("Self decrement")
      {
        v1.yx -= v1;
        REQUIRE(v1 == result1);
      }

      SECTION("Self decrement with another swizzler")
      {
        v1.yx -= v1.xy;
        REQUIRE(v1 == result1);
      }
    }

    SECTION("dot product")
    {
      const beyond::Vector2f v4{b, a};
      const auto result = 2 * a * b;

      CHECK(dot(v1.xy, v4.xy) == Approx(result));

      CHECK(dot(v1.xy, v4) == Approx(result));

      CHECK(dot(v1, v4.xy) == Approx(result));
    }
  }
}

TEST_CASE("Cross product", "[beyond.core.math.vec]")
{
  GIVEN("Two vectors")
  {
    const beyond::Vector3f a{1, 2, 3};
    const beyond::Vector3f b{2, 3, 4};
    THEN("cross(a,b) == [-1, 2, -1]")
    {
      const beyond::Vector3f result1 = beyond::cross(a, b);
      REQUIRE(result1.x == Approx(-1));
      REQUIRE(result1.y == Approx(2));
      REQUIRE(result1.z == Approx(-1));

      AND_THEN("cross(a.xyz, b.xyz) == cross(a, b)")
      {
        REQUIRE(beyond::cross(a.xyz, b.xyz) == beyond::cross(a, b));
      }

      AND_THEN("cross(a.xyz, b) == cross(a, b)")
      {
        REQUIRE(beyond::cross(a.xyz, b) == beyond::cross(a, b));
      }

      AND_THEN("cross(a, b.xyz) == cross(a, b)")
      {
        REQUIRE(beyond::cross(a, b.xyz) == beyond::cross(a, b));
      }
    }

    THEN("cross(b, a) == [1, -2, 1]")
    {
      const beyond::Vector3f result2 = beyond::cross(b, a);
      REQUIRE(result2.x == Approx(1));
      REQUIRE(result2.y == Approx(-2));
      REQUIRE(result2.z == Approx(1));
    }
  }
}

TEST_CASE("Vector serialization test", "[beyond.core.math.vec]")
{
  SECTION("Output Vector2 to a stream")
  {
    const beyond::Vector2f v{1, 2};
    const auto expected = "vec(1, 2)";

    REQUIRE_TO_STRING_EQ(v, expected);
  }

  SECTION("Output Vector3 to a stream")
  {
    const beyond::Vector3f v{1, 2, 3};
    const auto expected = "vec(1, 2, 3)";

    REQUIRE_TO_STRING_EQ(v, expected);
  }

  SECTION("Output Vector4 to a stream")
  {
    const beyond::Vector4f v{1, 2, 3, 4};
    const auto expected = "vec(1, 2, 3, 4)";

    REQUIRE_TO_STRING_EQ(v, expected);
  }
}

TEST_CASE("Point serialization test", "[beyond.core.math.vec]")
{
  SECTION("Output Point2 to a stream")
  {
    const beyond::Point2f p{1, 2};
    const auto expected = "point(1, 2)";

    REQUIRE_TO_STRING_EQ(p, expected);
  }

  SECTION("Output Point3 to a stream")
  {
    const beyond::Point3f p{1, 2, 3};
    const auto expected = "point(1, 2, 3)";

    REQUIRE_TO_STRING_EQ(p, expected);
  }
}
