#pragma once

#ifndef BEYOND_CORE_MATH_TRANSFORM_HPP
#define BEYOND_CORE_MATH_TRANSFORM_HPP

#include "angle.hpp"
#include "function.hpp"
#include "matrix.hpp"

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

/**
 * @brief Creates a matrix for an orthographic parallel viewing volume.
 */
template <typename T>
[[nodiscard]] constexpr auto ortho(T left, T right, T bottom, T top, T z_near,
                                   T z_far) -> TMat4<T>
{
  TMat4<T> Result;
  Result[0][0] = static_cast<T>(2) / (right - left);
  Result[1][1] = static_cast<T>(2) / (top - bottom);
  Result[2][2] = -static_cast<T>(2) / (z_far - z_near);
  Result[3][0] = -(right + left) / (right - left);
  Result[3][1] = -(top + bottom) / (top - bottom);
  Result[3][2] = -(z_far + z_near) / (z_far - z_near);
  Result[3][3] = static_cast<T>(1);
  return Result;
}

/** @}
 *  @}
 *  @} */
} // namespace beyond

#endif // BEYOND_CORE_MATH_TRANSFORM_HPP
