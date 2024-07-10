#ifndef BEYOND_CORE_MATH_HPP
#define BEYOND_CORE_MATH_HPP

/**
 * @file math.hpp
 * @brief This file contains various mathmatic utility functions
 * @ingroup math
 */

/**
 * @defgroup math Math
 * @brief Mathematics and geometry codes of the beyond game engine
 * @ingroup core
 */
#include "angle.hpp"
#include "concepts.hpp"

#include <cmath>
#include <type_traits>

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

/**
 * @brief Computes the sine of arg
 * @tparam Angle The type of the angle, can be either degree or radian
 * @note Unlike the standard library conterpart, this function only accept
 * either TDegree or TRadian
 * @see TDegree
 * @see TRadian
 */
template <typename Angle>
[[nodiscard]] inline auto sin(Angle arg) noexcept -> typename Angle::ValueType
{
  beyond::TRadian r{arg};
  return std::sin(r.value());
}

/**
 * @brief Computes the cosine of arg
 * @tparam Angle The type of the angle, can be either degree or radian
 * @note Unlike the standard library conterpart, this function only accept
 * either TDegree or TRadian
 * @see TDegree
 * @see TRadian
 */
template <typename Angle>
[[nodiscard]] inline auto cos(Angle arg) noexcept -> typename Angle::ValueType
{
  beyond::TRadian r{arg};
  return std::cos(r.value());
}

/**
 * @brief Computes the tangent of arg
 * @tparam Angle The type of the angle, can be either degree or radian
 * @note Unlike the standard library conterpart, this function only accept
 * either TDegree or TRadian
 * @see TDegree
 * @see TRadian
 */
template <typename Angle>
[[nodiscard]] inline auto tan(Angle arg) noexcept -> typename Angle::ValueType
{
  beyond::TRadian r{arg};
  return std::tan(r.value());
}

/**
 * @brief Computes the principal value of the arc sine of arg
 * @tparam T A floating-point type
 * @see TRadian
 */
template <std::floating_point T>
[[nodiscard]] inline auto asin(T arg) noexcept -> TRadian<T>
{
  return beyond::TRadian<T>{std::asin(arg)};
}

/**
 * @overload
 * @tparam T An integral type
 * @see TRadian
 */
template <std::integral T>
[[nodiscard]] inline auto asin(T arg) noexcept -> TRadian<double>
{
  return beyond::TRadian<double>{std::asin(arg)};
}

/**
 * @brief Computes the principal value of the arc cosine of arg
 * @tparam T A floating-point type
 * @see TRadian
 */
template <std::floating_point T>
[[nodiscard]] inline auto acos(T arg) noexcept -> TRadian<T>
{
  return beyond::TRadian<T>{std::acos(arg)};
}

/**
 * @overload
 * @tparam T An integral type
 * @see TRadian
 */
template <std::integral T>
[[nodiscard]] inline auto acos(T arg) noexcept -> TRadian<double>
{
  return beyond::TRadian<double>{std::acos(arg)};
}

/**
 * @brief Computes the principal value of the arc tangent of arg
 * @tparam T A floating-point type
 * @see TRadian
 */
template <std::floating_point T>
[[nodiscard]] inline auto atan(T arg) noexcept -> TRadian<T>
{
  return beyond::TRadian<T>{std::atan(arg)};
}

/**
 * @overload
 * @tparam T An integral type
 * @see TRadian
 */
template <std::integral T>
[[nodiscard]] inline auto atan(T arg) noexcept -> TRadian<double>
{
  return beyond::TRadian<double>{std::atan(arg)};
}

/**
 * @brief Computes the arc tangent of y/x using the signs of arguments to
 * determine the correct quadrant.
 * @tparam T1 A floating-point type for y
 * @tparam T2 A floating-point type for x
 * @see TRadian
 */
template <Arithmetic T1, Arithmetic T2>
[[nodiscard]] inline auto atan2(T1 y, T2 x) noexcept
{
  using PromotedType = std::common_type_t<T1, T2>;
  if constexpr (std::is_integral_v<PromotedType>) {
    return TRadian<double>{std::atan2(y, x)};
  } else {
    return TRadian<PromotedType>{std::atan2(y, x)};
  }
}

/**
 * @brief Linear interpolation of two values.
 * @param a First value
 * @param b Second value
 * @param t Interpolation phase (from range \f$[0, 1]\f$)
 */
[[nodiscard]] constexpr auto lerp(float a, float b, float t) noexcept -> float
{
  return a * (1.0f - t) + (b * t);
}

[[nodiscard]] constexpr auto lerp(double a, double b,
                                  double t) noexcept -> double
{
  return a * (1.0 - t) + (b * t);
}

/**
 * @brief A constexpr version of sqrt
 * @return For a finite and non-negative value of "x", returns an approximation
 *for the square root of "x" Otherwise, returns NaN
 */
template <std::floating_point T> [[nodiscard]] auto constexpr sqrt(T x) -> float
{
  if (std::is_constant_evaluated()) {
    if (x < 0) { return std::numeric_limits<T>::quiet_NaN(); }
    if (x == std::numeric_limits<T>::infinity()) {
      return std::numeric_limits<T>::infinity();
    }

    T curr = x;
    for (T prev = T{0}; curr != prev;) {
      prev = std::exchange(curr, T{0.5} * (curr + x / curr));
    }
    return curr;
  }

  return std::sqrt(x);
}

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_MATH_HPP
