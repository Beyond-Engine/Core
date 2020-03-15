#include "beyond/core/math/serial.hpp"
#include "beyond/core/math/transform.hpp"
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
  beyond::Mat4 expected{
      // clang-format off
      1.5, 0, 0, 0,
      0, 2.5, 0, 0,
      0, 0, 3.5, 0,
      0, 0, 0, 1
      // clang-format on
  };
  matrix_approx_match(beyond::scale(1.5f, 2.5f, 3.5f), expected);
}