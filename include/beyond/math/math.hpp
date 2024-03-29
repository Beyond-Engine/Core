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
  return a * (static_cast<float>(1) - t) + (b * t);
}

[[nodiscard]] constexpr auto lerp(double a, double b, double t) noexcept
    -> double
{
  return a * (static_cast<float>(1) - t) + (b * t);
}

using std::abs;
using std::fabs;
using std::fdim;
using std::fma;
using std::fmod;
using std::remainder;
using std::remquo;

using std::exp;
using std::exp2;
using std::expm1;
using std::log;
using std::log10;
using std::log1p;
using std::log2;

using std::cbrt;
using std::hypot;
using std::pow;
using std::sqrt;

using std::acosh;
using std::asinh;
using std::atanh;
using std::cosh;
using std::sinh;
using std::tanh;

using std::ceil;
using std::floor;
using std::nearbyint;
using std::round;
using std::trunc;

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_MATH_HPP
