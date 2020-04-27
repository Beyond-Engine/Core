#include "beyond/math/function.hpp"
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
  constexpr float left = 0, right = 800, bottom = 0, top = 600, z_near = 0,
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

TEST_CASE("Perspective Projection", "[beyond.core.math.transform]")
{
  using namespace beyond::literals;

  constexpr beyond::Radian fovy = 90._deg;
  constexpr float aspect = 1.6f, z_near = 0.f, z_far = 1.f;

  const auto result = beyond::perspective(fovy, aspect, z_near, z_far);

  beyond::Mat4 expected{};
  expected(0, 0) = 1.0f / aspect;
  expected(1, 1) = 1.0f;
  expected(3, 2) = -1.0f;
  expected(2, 2) = -1.0f;
  expected(2, 3) = 0.0f;

  matrix_approx_match(expected, result);
}

TEST_CASE("look_at transformation matrix", "[beyond.core.math.transform]")
{
  constexpr beyond::Vec3 eye{0, 10, 0}, center{0, 0, 0}, up_dir{0, 0, 1};

  const beyond::Vec3 forward(normalize(center - eye));
  const beyond::Vec3 side(normalize(cross(forward, up_dir)));
  const beyond::Vec3 up(cross(side, forward));

  const auto result = beyond::look_at(eye, center, up_dir);

  beyond::Mat4 expected{};
  expected[0][0] = side.x;
  expected[1][0] = side.y;
  expected[2][0] = side.z;
  expected[3][0] = -dot(side, eye);
  expected[0][1] = up.x;
  expected[1][1] = up.y;
  expected[2][1] = up.z;
  expected[3][1] = -dot(up, eye);
  expected[0][2] = -forward.x;
  expected[1][2] = -forward.y;
  expected[2][2] = -forward.z;
  expected[3][2] = dot(forward, eye);
  expected[3][3] = -1.0f;

  matrix_approx_match(result, expected);
}