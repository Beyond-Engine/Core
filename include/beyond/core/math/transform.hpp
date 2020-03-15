#pragma once

#ifndef BEYOND_CORE_MATH_TRANSFORM_HPP
#define BEYOND_CORE_MATH_TRANSFORM_HPP

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

/**
 * @defgroup transform Transform
 * @brief Matrix transformations
 * @ingroup math
 *
 * @{
 */

/// @brief Creates a 4x4 rotation matrix that rotates around x-axis  by r radian
template <typename T>
[[nodiscard]] inline auto rotate_x(TRadian<T> r) noexcept -> TMat4<T>
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

/// @brief Creates a 4x4 rotation matrix that rotates around x-axis  by r radian
template <typename T>
[[nodiscard]] inline auto rotate_y(TRadian<T> r) noexcept -> TMat4<T>
{
  const T s = beyond::sin(r);
  const T c = beyond::cos(r);

  return {
      // clang-format off
      c, 0, s, 0,
      0, 1, 0, 0,
      -s, 0, c, 0,
      0, 0, 0, 1
      // clang-format on
  };
}

/// @brief Creates a 4x4 rotation matrix that rotates around x-axis  by r radian
template <typename T>
[[nodiscard]] inline auto rotate_z(TRadian<T> r) noexcept -> TMat4<T>
{
  const T s = beyond::sin(r);
  const T c = beyond::cos(r);

  return {
      // clang-format off
      c, -s, 0, 0,
      s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
      // clang-format on
  };
}

// Rotation functions with degree as input
#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <typename T>
[[nodiscard]] inline auto rotate_x(TDegree<T> d) noexcept -> TMat4<T>
{
  return rotate_x(TRadian<T>(d));
}

template <typename T>
[[nodiscard]] inline auto rotate_y(TDegree<T> d) noexcept -> TMat4<T>
{
  return rotate_y(TRadian<T>(d));
}

template <typename T>
[[nodiscard]] inline auto rotate_z(TDegree<T> d) noexcept -> TMat4<T>
{
  return rotate_z(TRadian<T>(d));
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

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

/// @brief Builds a scale 4 * 4 matrix created from 3 scalars.
template <typename T>
[[nodiscard]] constexpr auto scale(T x, T y, T z) noexcept -> TMat4<T>
{
  return {
      // clang-format off
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1
      // clang-format on
  };
}

/// @brief Builds a scale 4 * 4 matrix created from a 3d vector.
template <typename T>
[[nodiscard]] constexpr auto scale(const TVec3<T>& v) noexcept -> TMat4<T>
{
  return scale(v.x, v.y, v.z);
}

/// @brief Transforms a matrix with a translation 4 * 4 matrix created from 3
/// scalars.
template <typename T>
[[nodiscard]] constexpr auto scale(const TMat4<T>& m, T x, T y, T z) noexcept
    -> TMat4<T>
{
  return scale(x, y, z) * m;
}

/** @}
 *  @}
 *  @} */
} // namespace beyond

#endif // BEYOND_CORE_MATH_TRANSFORM_HPP
