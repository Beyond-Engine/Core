#ifndef BEYOND_CORE_MATH_VECTOR_HPP
#define BEYOND_CORE_MATH_VECTOR_HPP

/**
 * @file vector.hpp
 * @brief Provides TVec and TPoint classes
 * @ingroup math
 */

#include <cmath>
#include <type_traits>
#include <utility>

#include "../utils/assert.hpp"
#include "../utils/bit_cast.hpp"
#include "detail/swizzle.hpp"
#include "math.hpp"
#include "math_fwd.hpp"

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

/**
 * @brief Traits for vector like entities that holds information about
 components and subparts
 */
template <typename Vector> struct VecTrait;

template <typename T, std::size_t N> struct VecTrait<TVec<T, N>> {
  using ValueType = T;
  using VectorType = TVec<T, N>;
  using PointType = TPoint<T, N>;
  template <typename U> using VectorTemplate = TVec<U, N>;
  static constexpr std::size_t size = N;
};

/**
 * @brief Storage of a vector-like object
 * @see TVec, TPoint, TVec
 *
 * This class storage class of vector-like objects, including
 * TVec, and TPoint.
 */
template <typename Derived, std::size_t size> struct VectorStorage;

template <typename Derived> struct VectorStorage<Derived, 2> {
  using Trait = VecTrait<Derived>;
  using ValueType = typename Trait::ValueType;

  union {
    ValueType elem[2]{};
    struct {
      ValueType x;
      ValueType y;
    };
    Subvector2<Trait, 0, 1> xy;
    Subvector2<Trait, 1, 0> yx;
  };
};

template <typename Derived> struct VectorStorage<Derived, 3> {
  using Trait = VecTrait<Derived>;
  using ValueType = typename Trait::ValueType;
  using Trait2 = VecTrait<TVec<ValueType, 2>>;

  union {
    ValueType elem[3]{};
    struct {
      ValueType x;
      ValueType y;
      ValueType z;
    };
    Subvector2<Trait2, 0, 1> xy;
    Subvector2<Trait2, 1, 0> yx;
    Subvector2<Trait2, 0, 2> xz;
    Subvector2<Trait2, 2, 0> zx;
    Subvector2<Trait2, 1, 2> yz;
    Subvector2<Trait2, 2, 1> zy;
    Subvector3<Trait, 0, 1, 2> xyz;
    Subvector3<Trait, 0, 2, 1> xzy;
    Subvector3<Trait, 1, 0, 2> yxz;
    Subvector3<Trait, 1, 2, 0> yzx;
    Subvector3<Trait, 2, 0, 1> zxy;
    Subvector3<Trait, 2, 1, 0> zyx;
  };
};

template <typename Derived> struct VectorStorage<Derived, 4> {
  using Trait = VecTrait<Derived>;
  using ValueType = typename Trait::ValueType;
  using Trait2 = VecTrait<TVec<ValueType, 2>>;
  using Trait3 = VecTrait<TVec<ValueType, 3>>;

  union {
    ValueType elem[4]{};
    struct {
      ValueType x;
      ValueType y;
      ValueType z;
      ValueType w;
    };
    Subvector2<Trait2, 0, 1> xy;
    Subvector2<Trait2, 1, 0> yx;
    Subvector2<Trait2, 0, 2> xz;
    Subvector2<Trait2, 2, 0> zx;
    Subvector2<Trait2, 1, 2> yz;
    Subvector2<Trait2, 2, 1> zy;
    Subvector2<Trait2, 0, 3> xw;
    Subvector2<Trait2, 1, 3> yw;
    Subvector2<Trait2, 2, 3> zw;
    Subvector2<Trait2, 3, 0> wx;
    Subvector2<Trait2, 3, 1> wy;
    Subvector2<Trait2, 3, 2> wz;

    Subvector3<Trait3, 0, 1, 2> xyz;
    Subvector3<Trait3, 0, 1, 3> xyw;
    Subvector3<Trait3, 0, 2, 1> xzy;
    Subvector3<Trait3, 0, 2, 3> xzw;
    Subvector3<Trait3, 0, 3, 1> xwy;
    Subvector3<Trait3, 0, 3, 2> xwz;
    Subvector3<Trait3, 1, 0, 2> yxz;
    Subvector3<Trait3, 1, 0, 3> yxw;
    Subvector3<Trait3, 1, 2, 0> yzx;
    Subvector3<Trait3, 1, 2, 3> yzw;
    Subvector3<Trait3, 1, 3, 0> ywx;
    Subvector3<Trait3, 1, 3, 2> ywz;
    Subvector3<Trait3, 2, 0, 1> zxy;
    Subvector3<Trait3, 2, 0, 3> zxw;
    Subvector3<Trait3, 2, 1, 0> zyx;
    Subvector3<Trait3, 2, 1, 3> zyw;
    Subvector3<Trait3, 2, 3, 0> zwx;
    Subvector3<Trait3, 2, 3, 1> zwy;
    Subvector3<Trait3, 3, 0, 1> wxy;
    Subvector3<Trait3, 3, 0, 2> wxz;
    Subvector3<Trait3, 3, 1, 0> wyx;
    Subvector3<Trait3, 3, 1, 2> wyz;
    Subvector3<Trait3, 3, 2, 0> wzx;
    Subvector3<Trait3, 3, 2, 1> wzy;

    Subvector4<Trait, 0, 1, 2, 3> xyzw;
    Subvector4<Trait, 0, 1, 3, 2> xywz;
    Subvector4<Trait, 0, 2, 1, 3> xzyw;
    Subvector4<Trait, 0, 2, 3, 1> xzwy;
    Subvector4<Trait, 0, 3, 1, 2> xwyz;
    Subvector4<Trait, 0, 3, 2, 1> xwzy;
    Subvector4<Trait, 1, 0, 2, 3> yxzw;
    Subvector4<Trait, 1, 0, 3, 2> yxwz;
    Subvector4<Trait, 1, 2, 0, 3> yzxw;
    Subvector4<Trait, 1, 2, 3, 0> yzwx;
    Subvector4<Trait, 1, 3, 0, 2> ywxz;
    Subvector4<Trait, 1, 3, 2, 0> ywzx;
    Subvector4<Trait, 2, 0, 1, 3> zxyw;
    Subvector4<Trait, 2, 0, 3, 1> zxwy;
    Subvector4<Trait, 2, 1, 0, 3> zyxw;
    Subvector4<Trait, 2, 1, 3, 0> zywx;
    Subvector4<Trait, 2, 3, 0, 1> zwxy;
    Subvector4<Trait, 2, 3, 1, 0> zwyx;
    Subvector4<Trait, 3, 0, 1, 2> wxyz;
    Subvector4<Trait, 3, 0, 2, 1> wxzy;
    Subvector4<Trait, 3, 1, 0, 2> wyxz;
    Subvector4<Trait, 3, 1, 2, 0> wyzx;
    Subvector4<Trait, 3, 2, 0, 1> wzxy;
    Subvector4<Trait, 3, 2, 1, 0> wzyx;
  };
};

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

/**
 * @brief This class serve as base class for TVec.
 *
 * This class contains all the possible operations on vectors.
 *
 * @see TVec, VectorStorage
 */
template <typename T, std::size_t N>
struct TVec : VectorStorage<TVec<T, N>, N> {
  using ValueType = T;
  using Storage = VectorStorage<TVec<T, N>, N>;

  constexpr TVec() noexcept = default;
  constexpr TVec(T xx, T yy) noexcept
    requires(N == 2)
      : Storage{{xx, yy}}
  {
  }

  constexpr TVec(const TVec<T, 2>& v, T zz) noexcept
    requires(N == 3)
      : Storage{{v[0], v[1], zz}}
  {
  }
  constexpr TVec(T xx, T yy, T zz) noexcept
    requires(N == 3)
      : Storage{{xx, yy, zz}}
  {
  }

  constexpr TVec(T xx, T yy, T zz, T ww) noexcept
    requires(N == 4)
      : Storage{{xx, yy, zz, ww}}
  {
  }

  constexpr TVec(const TVec<T, 2>& v, T zz, T ww) noexcept
    requires(N == 4)
      : Storage{{v[0], v[1], zz, ww}}
  {
  }

  constexpr TVec(const TVec<T, 3>& v, T ww) noexcept
    requires(N == 4)
      : Storage{{v[0], v[1], v[2], ww}}
  {
  }

  /**
   * @brief Gets the dimensionality of a vector
   */
  [[nodiscard]] static constexpr auto size() noexcept -> std::size_t
  {
    return N;
  }

  /// @brief Same as size()
  [[nodiscard]] static constexpr auto dimension() noexcept -> std::size_t
  {
    return N;
  }

  /**
   * @brief Gets the length of the vector
   * @note Only vectors of floating point components support this operation
   * @see length_squared()
   */
  template <typename U = ValueType>
  [[nodiscard]] auto length() const noexcept -> U
  {
    static_assert(std::is_floating_point_v<U>);
    return std::sqrt(length_squared());
  }

  /**
   * @brief Gets the i-th component of the vector
   * @warning Behavior of out of index is undefined
   */
  auto operator[](std::size_t i) const noexcept -> ValueType
  {
    BEYOND_ASSERT_MSG(i < size(), "Invalid index");
    return Storage::elem[i];
  }

  /// @overload
  auto operator[](std::size_t i) noexcept -> ValueType&
  {
    BEYOND_ASSERT_MSG(i < size(), "Invalid index");
    return Storage::elem[i];
  }

  /**
   * @brief Gets the i-th component of the vector
   * @warning Behavior of out of index is undefined
   */
  auto operator()(std::size_t i) const noexcept -> ValueType
  {
    BEYOND_ASSERT_MSG(i < size(), "Invalid index");
    return Storage::elem[i];
  }

  /// @overload
  auto operator()(std::size_t i) noexcept -> ValueType&
  {
    BEYOND_ASSERT_MSG(i < size(), "Invalid index");
    return Storage::elem[i];
  }

  /**
   * @brief Negates this vector
   */
  constexpr auto operator-() const noexcept -> TVec
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      return TVec{(-Storage::elem[I])...};
    }(std::make_index_sequence<N>());
  }

  /**
   * @brief Adds another vector to this vector
   */
  constexpr auto operator+=(const TVec& rhs) noexcept -> TVec&
  {
    [&]<std::size_t... I>(std::index_sequence<I...>) {
      ((Storage::elem[I] += rhs[I]), ...);
    }(std::make_index_sequence<N>());
    return *this;
  }

  /**
   * @brief Subtract another vector to this vector
   */
  constexpr auto operator-=(const TVec& rhs) noexcept -> TVec&
  {
    [&]<std::size_t... I>(std::index_sequence<I...>) {
      ((Storage::elem[I] -= rhs[I]), ...);
    }(std::make_index_sequence<N>());
    return *this;
  }

  /**
   * @brief Multiply a scalar to this vector
   */
  constexpr auto operator*=(ValueType rhs) noexcept -> TVec&
  {
    [&]<std::size_t... I>(std::index_sequence<I...>) {
      ((Storage::elem[I] *= rhs), ...);
    }(std::make_index_sequence<N>());
    return *this;
  }

  /**
   * @brief Divides this vector by a scalar
   */
  template <typename U = ValueType>
  constexpr auto operator/=(U rhs) noexcept -> TVec&
  {
    static_assert(std::is_floating_point_v<U>);
    BEYOND_ASSERT_MSG(rhs != 0, "Devide by zero");
    const auto inv = static_cast<ValueType>(1) / rhs;
    [&]<std::size_t... I>(std::index_sequence<I...>) {
      ((Storage::elem[I] *= inv), ...);
    }(std::make_index_sequence<N>());
    return *this;
  }

  /**
   * @brief Returns the squared length of this vector.
   *
   * The magnitude of vector `v` is calculated by `sqrt(dot(v, v))`. However,
   * square root function is relatively expensive.  If you are using `length`
   * simply to compare distances, then you can just as well compare squared
   * length.
   * @see length()
   */
  [[nodiscard]] auto length_squared() const noexcept -> ValueType
  {
    return dot(*this, *this);
  }

  /**
   * @brief Returns true if the given vector `v` is exactly equal to this
   * vector.
   * @warning Due to floating point inaccuracies, this might return false for
   * vectors which are essentially (but not exactly) equal.
   */
  [[nodiscard]] constexpr auto operator==(const TVec& v) const noexcept -> bool
  {
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      return ((Storage::elem[I] == v.elem[I]) && ...);
    }(std::make_index_sequence<N>());
  }

  /**
   * @brief Adds two vectors
   * @related TVec
   */
  [[nodiscard]] friend constexpr auto operator+(TVec lhs,
                                                const TVec& rhs) noexcept
      -> TVec
  {
    lhs += rhs;
    return lhs;
  }

  /**
   * @brief Subtracts two vectors
   * @related TVec
   */
  [[nodiscard]] friend constexpr auto operator-(TVec lhs,
                                                const TVec& rhs) noexcept
      -> TVec
  {
    lhs -= rhs;
    return lhs;
  }

  /**
   * @brief Divides a vector by a scalar
   * @related TVec
   */
  [[nodiscard]] friend constexpr auto operator/(TVec lhs, T scalar) noexcept
      -> TVec
  {
    static_assert(std::is_floating_point_v<T>);
    lhs /= scalar;
    return lhs;
  }
};

/// @brief Normalizes a vector into a unit vector
/// @note Only available for floating point vectors
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto normalize(const TVec<T, N>& v) noexcept
    -> TVec<T, N>
  requires(std::floating_point<T>)
{
  return v / v.length();
}

/**
 * @brief Multiplies vector `v` by a scalar
 * @related TVec
 */
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto operator*(const TVec<T, N>& v, T scalar) noexcept
{
  return [&]<std::size_t... I>(std::index_sequence<I...>) {
    return TVec<T, N>{(scalar * v.elem[I])...};
  }(std::make_index_sequence<N>());
}

/**
 * @brief Multiplies vector `v` by a scalar
 * @related TVec
 * @overload
 */
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto operator*(T scalar, const TVec<T, N>& v) noexcept
{
  return v * scalar;
}

/**
 * @brief Gets the Dot Product of two vectors
 * @related TVec
 *
 * Dot product takes two vectors of the same dimensionality and sum the
 * products of the corresponding components of the vectors. The result is a
 * scalar.
 */
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto dot(const TVec<T, N>& v1,
                                 const TVec<T, N>& v2) noexcept -> T
{
  return [&]<std::size_t... I>(std::index_sequence<I...>) {
    return ((v1.elem[I] * v2.elem[I]) + ...);
  }(std::make_index_sequence<N>());
}

/**
 * @brief Calculates cross product of two 3d vectors
 * @related TVec
 */
template <typename T>
[[nodiscard]] constexpr auto cross(const TVec<T, 3>& v1,
                                   const TVec<T, 3>& v2) noexcept -> TVec<T, 3>
{
  return {(v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z),
          (v1.x * v2.y) - (v1.y * v2.x)};
}

template <typename T, std::size_t size>
[[nodiscard]] constexpr auto lerp(const TVec<T, size>& v1,
                                  const TVec<T, size>& v2, T t) noexcept
    -> TVec<T, size>
{
  return [&]<std::size_t... I>(std::index_sequence<I...>) {
    TVec<T, size> v;
    ((v.elem[I] = beyond::lerp(v1.elem[I], v2.elem[I], t)), ...);
    return v;
  }(std::make_index_sequence<size>());
}

/** @}
 *  @} */

namespace detail {
[[nodiscard]] constexpr auto hash_combine(std::size_t seed, std::size_t hash)
    -> std::size_t
{
  return hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
} // namespace detail

} // namespace beyond

namespace std {

/**
 * @brief Hash support for TVec<T, N>
 */
template <typename T, std::size_t N> struct hash<beyond::TVec<T, N>> {
  [[nodiscard]] auto operator()(const beyond::TVec<T, N>& vec) const noexcept
      -> std::size_t
  {
    using beyond::detail::hash_combine;
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
      std::size_t seed = 0;
      ((seed ^= hash_combine(seed, std::hash<T>{}(vec[I]))), ...);
      return seed;
    }(std::make_index_sequence<N>());
  }
};

} // namespace std

#endif // BEYOND_CORE_MATH_VECTOR_HPP
