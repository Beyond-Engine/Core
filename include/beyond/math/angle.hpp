#ifndef BEYOND_CORE_MATH_ANGLE_HPP
#define BEYOND_CORE_MATH_ANGLE_HPP

#include <concepts>
#include <type_traits>
#include <utility>

#include "../utils/utils.hpp"
#include "constants.hpp"

/**
 * @file angle.hpp
 * @brief float point Degrees and Radians classes, along with literal operators.
 * @ingroup math
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

template <typename Value> class TDegree;

/**
 * @brief TRadian Radian is a number wrapper for angles
 * @see TDegree
 */
template <std::floating_point T> class TRadian {
public:
  using ValueType = T;

  constexpr TRadian() = default;
  explicit constexpr TRadian(T v) noexcept : value_{v} {}

  /// @brief Converts a TDegree to TRadian
  explicit(false) constexpr TRadian(TDegree<T> d) noexcept
      : value_{d.value() * constant::pi<T> / static_cast<T>(180)}
  {
  }

  /**
   * @brief Convert from radian of another underlying type
   */
  template <typename U>
  explicit constexpr TRadian(TRadian<U> r) noexcept
      : value_{static_cast<T>(r.value())}
  {
  }

  /// @brief Gets the underlying numerical value of the TRadian
  [[nodiscard]] constexpr auto value() const noexcept -> T
  {
    return value_;
  }

  /// @brief Adds another TRadian to this radian
  constexpr auto operator+=(TRadian<T> rhs) noexcept -> TRadian&
  {
    value_ += rhs.value_;
    return *this;
  }

  /// @brief Subtracts another TRadian to this radian
  constexpr auto operator-=(TRadian<T> rhs) noexcept -> TRadian&
  {
    value_ -= rhs.value_;
    return *this;
  }

  /// @brief Multiples a scalar to this radian
  constexpr auto operator*=(T rhs) noexcept -> TRadian&
  {
    value_ *= rhs;
    return *this;
  }

  /// @brief Divides this radian by a scalar
  constexpr auto operator/=(T rhs) noexcept -> TRadian&
  {
    value_ /= rhs;
    return *this;
  }

  [[nodiscard]] static constexpr auto pi() noexcept -> TRadian<T>
  {
    return TRadian<T>{constant::pi<T>};
  }

  /// @brief Negates the TRadian
  [[nodiscard]] friend constexpr auto operator-(TRadian r) noexcept -> TRadian
  {
    return TRadian{-r.value()};
  }

  /// @brief Adds two Radians
  [[nodiscard]] friend constexpr auto operator+(TRadian lhs,
                                                TRadian rhs) noexcept -> TRadian
  {
    return TRadian{lhs.value() + rhs.value()};
  }

  /// @brief Subtracts two Radians
  [[nodiscard]] friend constexpr auto operator-(TRadian lhs,
                                                TRadian rhs) noexcept -> TRadian
  {
    return TRadian{lhs.value() - rhs.value()};
  }

  /// @brief Multiplies TRadian with a scalar
  [[nodiscard]] friend constexpr auto operator*(TRadian lhs, T rhs) noexcept
      -> TRadian
  {
    return TRadian{lhs.value() * rhs};
  }

  /// @overload
  [[nodiscard]] friend constexpr auto operator*(T lhs, TRadian rhs) noexcept
      -> TRadian
  {
    return TRadian{lhs * rhs.value()};
  }

  /// @brief Divides this TRadian by a scalar
  [[nodiscard]] friend constexpr auto operator/(TRadian lhs, T rhs) noexcept
      -> TRadian
  {
    return TRadian{lhs.value() / rhs};
  }

  /**
   * @brief Divides a TRadian value by another TRadian.
   */
  [[nodiscard]] friend constexpr auto operator/(TRadian lhs,
                                                TRadian rhs) noexcept -> T
  {
    return lhs.value() / rhs.value();
  }

  /**
   * @brief Equality comparison between two `TRadian`s
   */
  [[nodiscard]] friend constexpr auto operator==(TRadian lhs,
                                                 TRadian rhs) noexcept -> bool
  {
    return lhs.value() == rhs.value();
  }

  /**
   * @brief Threeway comparison between two `TRadian`s
   */
  [[nodiscard]] friend constexpr auto operator<=>(TRadian lhs,
                                                  TRadian rhs) noexcept
      -> std::partial_ordering
  {
    return lhs.value() <=> rhs.value();
  }

private:
  T value_ = 0;
};

/**
 * @brief TDegree Degree is a number wrapper for angles
 * @see TRadian
 */
template <typename T> class TDegree {
public:
  static_assert(std::is_floating_point_v<T>);
  using ValueType = T;

  constexpr TDegree() = default;
  explicit constexpr TDegree(T v) noexcept : value_{v} {}

  explicit(false) constexpr TDegree(TRadian<T> r) noexcept
      : value_{r.value() / constant::pi<T> * 180}
  {
  }

  /**
   * @brief Convert from TDegree of another underlying type
   */
  template <typename U>
  explicit constexpr TDegree(TDegree<U> r) noexcept
      : value_{static_cast<T>(r.value())}
  {
  }

  /// @brief Gets the underlying numerical value of the TDegree
  [[nodiscard]] constexpr auto value() const noexcept -> T
  {
    return value_;
  }

  /// @brief Adds another TDegree to this TDegree
  constexpr auto operator+=(TDegree<T> rhs) noexcept -> TDegree&
  {
    value_ += rhs.value_;
    return *this;
  }

  /// @brief Subtracts another TDegree to this TDegree
  constexpr auto operator-=(TDegree<T> rhs) noexcept -> TDegree&
  {
    value_ -= rhs.value_;
    return *this;
  }

  /// @brief Multiples a scalar to this TDegree
  constexpr auto operator*=(T rhs) noexcept -> TDegree&
  {
    value_ *= rhs;
    return *this;
  }

  /// @brief Divides this TDegree by a scalar
  constexpr auto operator/=(T rhs) noexcept -> TDegree&
  {
    value_ /= rhs;
    return *this;
  }

  /// @brief Negates the TDegree
  [[nodiscard]] friend constexpr auto operator-(TDegree r) noexcept -> TDegree
  {
    return TDegree{-r.value()};
  }

  /// @brief Adds two Degrees
  [[nodiscard]] friend constexpr auto operator+(TDegree lhs,
                                                TDegree rhs) noexcept -> TDegree
  {
    return TDegree{lhs.value() + rhs.value()};
  }

  /// @brief Subtracts two Degrees
  [[nodiscard]] friend constexpr auto operator-(TDegree lhs,
                                                TDegree rhs) noexcept -> TDegree
  {
    return TDegree{lhs.value() - rhs.value()};
  }

  /// @brief Multiplies TDegree with a scalar
  [[nodiscard]] friend constexpr auto operator*(TDegree lhs, T rhs) noexcept
      -> TDegree
  {
    return TDegree{lhs.value() * rhs};
  }

  /// @overload
  [[nodiscard]] friend constexpr auto operator*(T lhs, TDegree rhs) noexcept
      -> TDegree
  {
    return TDegree{lhs * rhs.value()};
  }

  /// @brief Divides this TDegree by a scalar
  [[nodiscard]] friend constexpr auto operator/(TDegree lhs, T rhs) noexcept
      -> TDegree
  {
    return TDegree{lhs.value() / rhs};
  }

  /**
   * @brief Divides a TDegree value by another TDegree.
   */
  [[nodiscard]] friend constexpr auto operator/(TDegree lhs,
                                                TDegree rhs) noexcept -> T
  {
    return lhs.value() / rhs.value();
  }

  /**
   * @brief Equality comparison between two `TDegree`s
   */
  [[nodiscard]] friend constexpr auto operator==(TDegree lhs,
                                                 TDegree rhs) noexcept -> bool
  {
    return lhs.value() == rhs.value();
  }

  /**
   * @brief Three-way comparison between two `TDegree`s
   */
  [[nodiscard]] friend constexpr auto operator<=>(TDegree lhs,
                                                  TDegree rhs) noexcept
      -> std::partial_ordering
  {
    return lhs.value() <=> rhs.value();
  }

private:
  T value_ = 0;
};

template <typename T> [[nodiscard]] auto to_degree(TRadian<T> rad) -> TDegree<T>
{
  return TDegree{rad};
}

template <typename T> [[nodiscard]] auto to_radian(TDegree<T> deg) -> TRadian<T>
{
  return TRadian{deg};
}

namespace literals {

/// @brief Construct a TRadian<float> by a literial
/// @related TRadian
[[nodiscard]] constexpr auto operator""_rad(long double v) noexcept
    -> TRadian<float>
{
  return TRadian<float>(static_cast<float>(v));
}

/// @brief Construct a TDegree<float> by a literial
/// @related TDegree
[[nodiscard]] constexpr auto operator""_deg(long double v) noexcept
    -> TDegree<float>
{
  return TDegree<float>(static_cast<float>(v));
}

} // namespace literals

using Radian = TRadian<float>;
using DRadian = TRadian<double>;
using Degree = TDegree<float>;
using DDegree = TDegree<double>;

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_MATH_ANGLE_HPP
