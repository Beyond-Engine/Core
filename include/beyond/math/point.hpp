#ifndef BEYOND_CORE_MATH_POINT_HPP
#define BEYOND_CORE_MATH_POINT_HPP

#include "concepts.hpp"
#include "math.hpp"
#include "math_fwd.hpp"
#include "vector.hpp"

/**
 * @file point.hpp
 * @brief Provides TPoint classes
 * @ingroup math
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

template <typename T, std::size_t N> struct TPoint : TVec<T, N> {
  using TVec<T, N>::TVec;

  /**
   * @brief TPoint + TVec = TPoint
   */
  [[nodiscard]] friend constexpr auto operator+(TPoint p,
                                                const TVec<T, N>& v) noexcept
      -> TPoint
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      ((p[I] += v[I]), ...);
      return p;
    }(std::make_index_sequence<N>());
  }

  /**
   * @brief TVec + TPoint = TPoint
   */
  [[nodiscard]] friend constexpr auto operator+(const TVec<T, N>& v,
                                                TPoint p) noexcept -> TPoint
  {
    return p + v;
  }

  /**
   * @brief TPoint - TVec = TPoint
   */
  [[nodiscard]] friend constexpr auto operator-(TPoint p,
                                                const TVec<T, N>& v) noexcept
      -> TPoint
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      ((p[I] -= v[I]), ...);
      return p;
    }(std::make_index_sequence<N>());
  }

  /**
   * @brief TPoint - TPoint = TVec
   */
  [[nodiscard]] friend constexpr auto operator-(const TPoint& p1,
                                                const TPoint& p2) noexcept
      -> TVec<T, N>
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      TVec v = p1;
      ((v[I] -= p2[I]), ...);
      return v;
    }(std::make_index_sequence<N>());
  }

  /**
   * @brief TPoint + TPoint is not allowed
   */
  friend constexpr auto operator+(TPoint p1, TPoint p2) noexcept = delete;
};

template <typename T, std::size_t N, typename U>
auto dot(TPoint<T, N> p1, TVec<U, N> p2) = delete;

template <typename T, std::size_t N, typename U>
auto dot(TVec<T, N> p1, TPoint<U, N> p2) = delete;

template <typename T, typename U>
auto cross(TPoint3<T> p1, TVec3<U> p2) = delete;

template <typename T, typename U>
auto cross(TVec3<T> p1, TPoint3<U> p2) = delete;

template <typename T, std::size_t N, Arithmetic Scalar>
auto operator*(TPoint<T, N> p, Scalar scalar) = delete;

template <typename T, std::size_t N, Arithmetic Scalar>
auto operator*(Scalar scalar, TPoint<T, N> p) = delete;

template <typename T, std::size_t N> auto normalize(TPoint<T, N> p) = delete;

// TODO: delete scalar division, negation?, .length()

/**
 * @brief Gets the squared distance between two points
 */
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto distance_squared(const TPoint<T, N>& p1,
                                              const TPoint<T, N>& p2) noexcept
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

template <typename T, std::size_t size>
[[nodiscard]] constexpr auto
lerp(const TPoint<T, size>& p1, const TVec<T, size>& p2, T t) noexcept = delete;

template <typename T, std::size_t size>
[[nodiscard]] constexpr auto
lerp(const TVec<T, size>& p1, const TPoint<T, size>& p2, T t) noexcept = delete;

template <typename T, std::size_t size>
[[nodiscard]] constexpr auto lerp(const TPoint<T, size>& p1,
                                  const TPoint<T, size>& p2, T t) noexcept
    -> TPoint<T, size>
{
  return [&]<std::size_t... I>(std::index_sequence<I...>) {
    TPoint<T, size> v;
    ((v[I] = beyond::lerp(p1[I], p2[I], t)), ...);
    return v;
  }(std::make_index_sequence<size>());
}

/** @}
 *  @} */

} // namespace beyond

#endif