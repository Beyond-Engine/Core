#include "beyond/core/math/matrix.hpp"
namespace beyond {
/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

/// @brief Builds a translation 4 * 4 matrix created from 3 scalars.
template <typename T>
[[nodiscard]] constexpr auto translate(T x, T y, T z) noexcept -> Matrix4<T>
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
[[nodiscard]] constexpr auto translate(const Vector3<T>& v) noexcept
    -> Matrix4<T>
{
  return translate(v.x, v.y, v.z);
}

/// @brief Transforms a matrix with a translation 4 * 4 matrix created from 3
/// scalars.
template <typename T>
[[nodiscard]] constexpr auto translate(const Matrix4<T>& m, T x, T y,
                                       T z) noexcept -> Matrix4<T>
{
  return translate(x, y, z) * m;
}

/** @}
 *  @} */
} // namespace beyond

#include <catch2/catch.hpp>
#include "beyond/core/math/serial.hpp"

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