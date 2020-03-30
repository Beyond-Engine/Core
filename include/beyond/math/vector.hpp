#ifndef BEYOND_CORE_MATH_VECTOR_HPP
#define BEYOND_CORE_MATH_VECTOR_HPP

/**
 * @file vector.hpp
 * @brief Provides TVec and TPoint classes
 * @ingroup math
 */

#include <array>
#include <cmath>
#include <type_traits>
#include <utility>

#include "../utils/assert.hpp"
#include "../utils/bit_cast.hpp"
#include "detail/swizzle.hpp"
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
template <typename Vector> struct VectorTrait;

template <typename Value> struct VectorTrait<TVec<Value, 2>> {
  using ValueType = Value;
  using VectorType = TVec<Value, 2>;
  using PointType = TPoint<Value, 2>;
  template <typename T> using VectorTemplate = TVec<T, 2>;
  static constexpr std::size_t size = 2;
};

template <typename Value> struct VectorTrait<TVec<Value, 3>> {
  using ValueType = Value;
  using VectorType = TVec<Value, 3>;
  using PointType = TPoint<Value, 3>;
  template <typename T> using VectorTemplate = TVec<T, 3>;
  static constexpr std::size_t size = 3;
};

template <typename Value> struct VectorTrait<TVec<Value, 4>> {
  using ValueType = Value;
  using VectorType = TVec<Value, 4>;
  using PointType = TPoint<Value, 4>;
  template <typename T> using VectorTemplate = TVec<T, 4>;
  static constexpr std::size_t size = 4;
};

/**
 * @brief Storage of a vector-like object
 * @see TVec, TPoint, VectorBase
 *
 * This class storage class of vector-like objects, including
 * TVec, and TPoint.
 */
template <typename Derived, std::size_t size> struct VectorStorage;

template <typename Derived> struct VectorStorage<Derived, 2> {
  using Trait = VectorTrait<Derived>;
  using ValueType = typename Trait::ValueType;

  constexpr VectorStorage() noexcept : elem{{}} {}
  explicit constexpr VectorStorage(
      const std::array<ValueType, 2>& data) noexcept
      : elem{data}
  {
  }

  union {
    std::array<ValueType, 2> elem;
    struct {
      ValueType x;
      ValueType y;
    };
    Subvector2<Trait, 0, 1> xy;
    Subvector2<Trait, 1, 0> yx;
  };
};

template <typename Derived> struct VectorStorage<Derived, 3> {
  using Trait = VectorTrait<Derived>;
  using ValueType = typename Trait::ValueType;
  using Trait2 = VectorTrait<TVec<ValueType, 2>>;

  constexpr VectorStorage() noexcept : elem{{}} {}
  explicit constexpr VectorStorage(
      const std::array<ValueType, 3>& data) noexcept
      : elem{data}
  {
  }

  union {
    std::array<ValueType, 3> elem;
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
  using Trait = VectorTrait<Derived>;
  using ValueType = typename Trait::ValueType;
  using Trait2 = VectorTrait<TVec<ValueType, 2>>;
  using Trait3 = VectorTrait<TVec<ValueType, 3>>;

  constexpr VectorStorage() noexcept : elem{{}} {}
  explicit constexpr VectorStorage(
      const std::array<ValueType, 4>& data) noexcept
      : elem{data}
  {
  }
  union {
    std::array<ValueType, 4> elem;
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
 * @brief This class serve as base class for TVec, Bivector, and Trivectors.
 *
 * This class contains all the possible operations on vectors and anti-vectors.
 *
 * @see TVec, VectorStorage
 */
template <typename T, std::size_t... Ns>
struct VectorBase : VectorStorage<TVec<T, sizeof...(Ns)>, sizeof...(Ns)> {
  using ValueType = T;
  using Storage = VectorStorage<TVec<T, sizeof...(Ns)>, sizeof...(Ns)>;
  using VecType = TVec<T, sizeof...(Ns)>;

  /**
   * @brief Gets the dimensionality of a vector
   */
  [[nodiscard]] static constexpr auto size() noexcept -> std::size_t
  {
    return sizeof...(Ns);
  }

  constexpr VectorBase() noexcept = default;

  template <typename A0, typename... Args>
  explicit constexpr VectorBase(A0 a0, Args... args) noexcept
      : Storage{{a0, args...}}
  {
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
  constexpr auto operator-() const noexcept -> VectorBase
  {
    return VectorBase{(-Storage::elem[Ns])...};
  }

  /**
   * @brief Adds another vector to this vector
   */
  constexpr auto operator+=(const VectorBase& rhs) noexcept -> VectorBase&
  {
    ((Storage::elem[Ns] += rhs[Ns]), ...);
    return *this;
  }

  /**
   * @brief Subtract another vector to this vector
   */
  constexpr auto operator-=(const VectorBase& rhs) noexcept -> VectorBase&
  {
    ((Storage::elem[Ns] -= rhs[Ns]), ...);
    return *this;
  }

  /**
   * @brief Multiply a scalar to this vector
   */
  constexpr auto operator*=(ValueType rhs) noexcept -> VectorBase&
  {
    ((Storage::elem[Ns] *= rhs), ...);
    return *this;
  }

  /**
   * @brief Divides this vector by a scalar
   */
  template <typename U = ValueType>
  constexpr auto operator/=(U rhs) noexcept -> VectorBase&
  {
    static_assert(std::is_floating_point_v<U>);
    BEYOND_ASSERT_MSG(rhs != 0, "Devide by zero");
    const auto inv = static_cast<ValueType>(1) / rhs;
    ((Storage::elem[Ns] *= inv), ...);
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
  [[nodiscard]] constexpr auto operator==(const VectorBase& v) const noexcept
      -> bool
  {
    return ((Storage::elem[Ns] == v.elem[Ns]) && ...);
  }

  /**
   * @brief Returns false if the given vector `v` is exactly equal to this
   * vector.
   * @warning Due to floating point inaccuracies, this might return true for
   * vectors which are essentially (but not exactly) equal.
   */
  [[nodiscard]] constexpr auto operator!=(const VectorBase& v) const noexcept
      -> bool
  {
    return ((Storage::elem[Ns] != v.elem[Ns]) || ...);
  }
};

/**
 * @brief Adds two vectors
 * @related VectorBase
 */
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto operator+(VectorBase<T, Ns...> lhs,
                                       const VectorBase<T, Ns...>& rhs) noexcept
    -> typename VectorBase<T, Ns...>::VecType
{
  lhs += rhs;
  return lhs;
}

/**
 * @brief Subtracts two vectors
 * @related VectorBase
 */
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto operator-(VectorBase<T, Ns...> lhs,
                                       const VectorBase<T, Ns...>& rhs) noexcept
    -> typename VectorBase<T, Ns...>::VecType
{
  lhs -= rhs;
  return lhs;
}

/**
 * @brief Divides a vector by a scalar
 * @related VectorBase
 */
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto operator/(VectorBase<T, Ns...> lhs,
                                       T scalar) noexcept ->
    typename VectorBase<T, Ns...>::VecType
{
  static_assert(std::is_floating_point_v<T>);
  lhs /= scalar;
  return lhs;
}

/// @brief Normalizes a vector into a unit vector
/// @note Only available for floating point vectors
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto normalize(const VectorBase<T, Ns...>& v) noexcept
    -> TVec<T, sizeof...(Ns)>
{
  static_assert(std::is_floating_point_v<T>);
  return v / v.length();
}

template <typename T> struct TVec<T, 2> : VectorBase<T, 0, 1> {
  using ValueType = T;
  using Base = VectorBase<T, 0, 1>;

  constexpr TVec() noexcept = default;
  constexpr TVec(T xx, T yy) noexcept : Base{xx, yy} {}

  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr TVec(const Base& base) noexcept : Base{base} {}
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr TVec(Base&& base) noexcept : Base{std::forward<Base>(base)} {}

  [[nodiscard]] static constexpr auto dimension() noexcept -> std::size_t
  {
    return 2;
  }
};

template <typename T> struct TVec<T, 3> : VectorBase<T, 0, 1, 2> {
  using ValueType = T;
  using Base = VectorBase<T, 0, 1, 2>;

  constexpr TVec() noexcept = default;
  constexpr TVec(const TVec<T, 2>& v, T zz) noexcept : Base{v[0], v[1], zz} {}
  constexpr TVec(T xx, T yy, T zz) noexcept : Base{xx, yy, zz} {}

  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr TVec(const Base& base) noexcept : Base{base} {}
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr TVec(Base&& base) noexcept : Base{std::forward<Base>(base)} {}

  [[nodiscard]] static constexpr auto dimension() noexcept -> std::size_t
  {
    return 3;
  }
};

template <typename T> struct TVec<T, 4> : VectorBase<T, 0, 1, 2, 3> {
  using ValueType = T;
  using Base = VectorBase<T, 0, 1, 2, 3>;

  constexpr TVec() noexcept = default;
  constexpr TVec(T xx, T yy, T zz, T ww) noexcept : Base{xx, yy, zz, ww} {}

  constexpr TVec(const TVec<T, 2>& v, T zz, T ww) noexcept
      : Base{v[0], v[1], zz, ww}
  {
  }

  constexpr TVec(const TVec<T, 3>& v, T ww) noexcept
      : Base{v[0], v[1], v[2], ww}
  {
  }

  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr TVec(const Base& base) noexcept : Base{base} {}
  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr TVec(Base&& base) noexcept : Base{std::forward<Base>(base)} {}

  [[nodiscard]] static constexpr auto dimension() noexcept -> std::size_t
  {
    return 4;
  }
};

/**
 * @brief Multiplies vector `v` by a scalar
 * @related VectorBase
 */
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto operator*(const VectorBase<T, Ns...>& v,
                                       T scalar) noexcept
{
  return VectorBase<T, Ns...>{(scalar * v.elem[Ns])...};
}

/**
 * @brief Multiplies vector `v` by a scalar
 * @related VectorBase
 * @overload
 */
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto operator*(T scalar,
                                       const VectorBase<T, Ns...>& v) noexcept
{
  return VectorBase<T, Ns...>{(scalar * v.elem[Ns])...};
}

/**
 * @brief Gets the Dot Product of two vectors
 * @related VectorBase
 *
 * Dot product takes two vectors of the same dimensionality and sum the products
 * of the corresponding components of the vectors. The result is a scalar.
 */
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto dot(const VectorBase<T, Ns...>& v1,
                                 const VectorBase<T, Ns...>& v2) noexcept -> T
{
  return ((v1.elem[Ns] * v2.elem[Ns]) + ...);
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

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_MATH_VECTOR_HPP
