#include "beyond/math/serial.hpp"
#include "beyond/math/transform.hpp"
#include "matrix_test_util.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Translate", "[beyond.core.math.transform]")
{
  SECTION("Create translation vector")
  {
    beyond::Mat4 expected{
        // clang-format off
        1, 0, 0, 1,
        0, 1, 0, 2,
        0, 0, 1, 3,
        0, 0, 0, 1
        // clang-format on
    };
    REQUIRE(beyond::translate(1.f, 2.f, 3.f) == expected);
    REQUIRE(beyond::translate(beyond::Vec3{1.f, 2.f, 3.f}) == expected);
  }

  SECTION("Translate existing matrix")
  {
    beyond::Mat4 m{
        // clang-format off
        2, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 2, 1,
        0, 0, 0, 1
        // clang-format on
    };

    beyond::Mat4 expected{
        // clang-format off
        2, 0, 0, 1,
        0, 2, 0, 2,
        0, 0, 2, 4,
        0, 0, 0, 1
        // clang-format on
    };

    REQUIRE(beyond::translate(m, 1.f, 2.f, 3.f) == expected);
  }
}

TEST_CASE("Axis-wise rotation", "[beyond.core.math.transform]")
{
  using namespace beyond::literals;

  const auto c = 0.5f;
  const auto s = std::sqrt(3.f) / 2.f;

  SECTION("rotate_x")
  {
    beyond::Mat4 expected{
        // clang-format off
        1, 0, 0, 0,
        0, c, -s, 0,
        0, s, c, 0,
        0, 0, 0, 1
        // clang-format on
    };

    matrix_approx_match(beyond::rotate_x(beyond::Radian::pi() / 3.f), expected);
    matrix_approx_match(beyond::rotate_x(60.0_deg), expected);
  }

  SECTION("rotate_y")
  {
    beyond::Mat4 expected{
        // clang-format off
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1
        // clang-format on
    };

    matrix_approx_match(beyond::rotate_y(beyond::Radian::pi() / 3.f), expected);
    matrix_approx_match(beyond::rotate_y(60.0_deg), expected);
  }

  SECTION("rotate_z")
  {
    beyond::Mat4 expected{
        // clang-format off
        c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        // clang-format on
    };

    matrix_approx_match(beyond::rotate_z(beyond::Radian::pi() / 3.f), expected);
    matrix_approx_match(beyond::rotate_z(60.0_deg), expected);
  }
}

TEST_CASE("Scaling", "[beyond.core.math.transform]")
{
  SECTION("Creates scaling matrix")
  {
    beyond::Mat4 expected{
        // clang-format off
        1.5, 0, 0, 0,
        0, 2.5, 0, 0,
        0, 0, 3.5, 0,
        0, 0, 0, 1
        // clang-format on
    };
    matrix_approx_match(beyond::scale(1.5f, 2.5f, 3.5f), expected);
    matrix_approx_match(beyond::scale(beyond::Vec3{1.5f, 2.5f, 3.5f}),
                        expected);
  }

  SECTION("Scaling existing matrix")
  {
    beyond::Mat4 m{
        // clang-format off
        2, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 2, 0,
        0, 0, 0, 1
        // clang-format on
    };

    beyond::Mat4 expected{
        // clang-format off
        3, 0, 0, 0,
        0, 5, 0, 0,
        0, 0, 7, 0,
        0, 0, 0, 1
        // clang-format on
    };

    REQUIRE(beyond::scale(m, 1.5f, 2.5f, 3.5f) == expected);
  }
}

TEST_CASE("Apply transformations together",
          "[.][beyond.core.math.transform][integration]")
{
  const auto A = beyond::rotate_x(beyond::Radian::pi() / 2.f);
  const auto B = beyond::scale(5.f, 5.f, 5.f);
  const auto C = beyond::translate(10.f, 5.f, 7.f);

  const auto v = beyond::Vec4{1, 0, 1, 1};
  const auto v1 = A * v;
  vector_approx_match(v1, beyond::Vec4{1, -1, 0, 1});
  const auto v2 = B * v1;
  vector_approx_match(v2, beyond::Vec4{5, -5, 0, 1});
  const auto v3 = C * v2;
  vector_approx_match(v3, beyond::Vec4{15, 0, 7, 1});

  vector_approx_match(v3, C * B * A * v);
}

TEST_CASE("Orthogonal Projection", "[beyond.core.math.transform]")
{
  const float left = 0, right = 800, bottom = 0, top = 600, z_near = 0,
              z_far = 1;

  const auto result = beyond::ortho(left, right, bottom, top, z_near, z_far);

  beyond::Mat4 expected{
      // clang-format off
      2.f / (right - left), 0,                    0,                       -(right + left) / (right - left),
      0,                    2.f / (top - bottom), 0,                       -(top + bottom) / (top - bottom),
      0,                    0,                    -2.f / (z_far - z_near), -(z_far + z_near) / (z_far - z_near),
      0,                    0,                    0,                       1
      // clang-format on
  };

  matrix_approx_match(expected, result);
}