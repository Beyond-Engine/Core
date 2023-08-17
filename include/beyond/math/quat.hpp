#ifndef BEYOND_CORE_MATH_QUAT_HPP
#define BEYOND_CORE_MATH_QUAT_HPP

#include "vector.hpp"

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

/**
 * @brief A quaternion type
 */
template <typename T> struct TQuat {
  T x, y, z, w;

  /// @brief Default constructor
  constexpr TQuat() noexcept : x{0}, y{0}, z{0}, w{0} {}

  /// @brief Component-wise constructor
  explicit constexpr TQuat(T w_, T x_, T y_, T z_) noexcept
      : x{x_}, y{y_}, z{z_}, w{w_}
  {
  }

  /// @brief Creates quaternion from a scalar and a vector
  explicit constexpr TQuat(T real, const TVec3<T>& v) noexcept
      : x{v.elem[0]}, y{v.elem[1]}, z{v.elem[2]}, w{real}
  {
  }

  /// @brief Quaternion multiplication
  [[nodiscard]] friend constexpr auto operator*(const TQuat<T>& lhs,
                                                const TQuat<T>& rhs) noexcept
      -> TQuat<T>
  {
    return TQuat<T>(
        lhs.w * rhs.x - lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
  }

  [[nodiscard]] friend constexpr auto operator==(const TQuat<T>& lhs,
                                                 const TQuat<T>& rhs) noexcept
      -> bool = default;
};

using Quat = TQuat<float>;
using DQuat = TQuat<double>;

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_MATH_QUAT_HPP
