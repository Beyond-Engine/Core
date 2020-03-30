#ifndef BEYOND_CORE_MATH_POINT_HPP
#define BEYOND_CORE_MATH_POINT_HPP

#include "math_fwd.hpp"
#include "vector.hpp"

/**
 * @file point.hpp
 * @brief Provides TPoint classes
 * @ingroup math
 */

namespace beyond {

template <typename T> struct TPoint<T, 2> : TVec<T, 2> {
  using TVec<T, 2>::TVec;
};

template <typename T> struct TPoint<T, 3> : TVec<T, 3> {
  using TVec<T, 3>::TVec;

  constexpr TPoint(const TPoint<T, 2>& p, T zz) noexcept : TPoint{p.x, p.y, zz}
  {
  }
};

/**
 * @brief TPoint + TVec = TPoint
 * @related TPoint
 */
template <typename T, std::size_t size>
[[nodiscard]] constexpr auto operator+(const TPoint<T, size>& p,
                                       const TVec<T, size>& v) noexcept
    -> TPoint<T, size>
{
  return static_cast<TPoint<T, size>>((static_cast<TVec<T, size>>(p) + v));
}

/**
 * @brief TVec + TPoint = TPoint
 * @related TPoint
 */
template <typename T, std::size_t size>
[[nodiscard]] constexpr auto operator+(const TVec<T, size>& v,
                                       const TPoint<T, size>& p) noexcept
    -> TPoint<T, size>
{
  return static_cast<TPoint<T, size>>((static_cast<TVec<T, size>>(p) + v));
}

/**
 * @brief Gets the squared distance between two points
 */
template <typename T, std::size_t size>
[[nodiscard]] constexpr auto
distance_squared(const TPoint<T, size>& p1, const TPoint<T, size>& p2) noexcept
    -> T
{
  const auto dx = p2 - p1;
  return dot(dx, dx);
}

/**
 * @brief Gets the distance between two points
 */
template <typename T, std::size_t size>
[[nodiscard]] auto distance(const TPoint<T, size>& p1,
                            const TPoint<T, size>& p2) noexcept -> T
{
  return std::sqrt(distance_squared(p1, p2));
}

} // namespace beyond

/** @}
 *  @} */

#endif