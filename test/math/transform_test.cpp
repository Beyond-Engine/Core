#include "beyond/core/math/angle.hpp"
#include "beyond/core/math/function.hpp"
#include "beyond/core/math/matrix.hpp"

namespace beyond {
/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

template <typename T>
[[nodiscard]] inline auto rotate_x(Radian<T> r) noexcept -> TMat4<T>
{
  const T s = beyond::sin(r);
  const T c = beyond::cos(r);

  return {
      // clang-format off
      1, 0, 0, 0,
      0, c, -s, 0,
      0, s, c, 0,
      0, 0, 0, 1
      // clang-format on
  };
}

template <typename T>
[[nodiscard]] inline auto rotate_x(Degree<T> d) noexcept -> TMat4<T>
{
  return rotate_x(Radian<T>(d));
}

/// @brief Builds a translation 4 * 4 matrix created from 3 scalars.
template <typename T>
[[nodiscard]] constexpr auto translate(T x, T y, T z) noexcept -> TMat4<T>
{
  return {
      // clang-format off
      1, 0, 0, x,
      0, 1, 0, y,
      0, 0, 1, z,
      0, 0, 0, 1
      // clang-format on
  };
}

/// @brief Builds a translation 4 * 4 matrix created from a 3d vector.
template <typename T>
[[nodiscard]] constexpr auto translate(const TVec3<T>& v) noexcept -> TMat4<T>
{
  return translate(v.x, v.y, v.z);
}

/// @brief Transforms a matrix with a translation 4 * 4 matrix created from 3
/// scalars.
template <typename T>
[[nodiscard]] constexpr auto translate(const TMat4<T>& m, T x, T y,
                                       T z) noexcept -> TMat4<T>
{
  return translate(x, y, z) * m;
}

/** @}
 *  @} */
} // namespace beyond

#include <catch2/catch.hpp>
#include "beyond/core/math/serial.hpp"
#include "matrix_test_util.hpp"

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
  const float sqrt3_over_2 = sqrt(3.f) / 2.f;
  beyond::Mat4 expected{
      // clang-format off
      1, 0, 0, 0,
      0, 0.5, -sqrt3_over_2, 0,
      0, sqrt3_over_2, 0.5, 0,
      0, 0, 0, 1
      // clang-format on
  };

  using namespace beyond::literals;
  matrix_approx_match(
      beyond::rotate_x(beyond::Radian(beyond::float_constants::pi) / 3.f),
      expected);
  matrix_approx_match(beyond::rotate_x(60.0_deg), expected);
}