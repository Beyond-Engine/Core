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
#include "../utils/functional.hpp"
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
  template <typename T> using VectorTemplate = TVec<T, 2>;
  static constexpr std::size_t size = 2;
};

template <typename Value> struct VectorTrait<TVec<Value, 3>> {
  using ValueType = Value;
  using VectorType = TVec<Value, 3>;
  template <typename T> using VectorTemplate = TVec<T, 3>;
  static constexpr std::size_t size = 3;
};

template <typename Value> struct VectorTrait<TVec<Value, 4>> {
  using ValueType = Value;
  using VectorType = TVec<Value, 4>;
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

namespace detail {

template <typename Trait, std::size_t... indices> struct VectorConverter;

template <typename Trait, std::size_t index_x, std::size_t index_y>
struct VectorConverter<Trait, index_x, index_y> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> VectorType
  {
    return VectorType(data[index_x], data[index_y]);
  }
};

template <typename Trait> struct VectorConverter<Trait, 0, 1> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*bit_cast<const VectorType*>(data));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) -> VectorType&
  {
    return (*bit_cast<VectorType*>(data));
  }
};

template <typename Trait> struct VectorConverter<Trait, 1, 2> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*bit_cast<const VectorType*>(data + 1)); // NOLINT
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) -> VectorType&
  {
    return (bit_cast<VectorType>(data + 1)); // NOLINT
  }
};

template <typename Trait> struct VectorConverter<Trait, 2, 3> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*bit_cast<const VectorType*>(data + 2)); // NOLINT
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) -> VectorType&
  {
    return (bit_cast<VectorType>(data + 2)); // NOLINT
  }
};

template <typename Trait, std::size_t index_x, std::size_t index_y,
          std::size_t index_z>
struct VectorConverter<Trait, index_x, index_y, index_z> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> VectorType
  {
    return VectorType(data[index_x], data[index_y], data[index_z]);
  }
};

template <typename Trait> struct VectorConverter<Trait, 0, 1, 2> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*bit_cast<const VectorType*>(data));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) -> VectorType&
  {
    return (*bit_cast<VectorType*>(data));
  }
};

template <typename Trait> struct VectorConverter<Trait, 1, 2, 3> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*bit_cast<const VectorType*>(data + 1));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) -> VectorType&
  {
    return (*bit_cast<VectorType*>(data + 1));
  }
};

template <typename Trait, std::size_t index_x, std::size_t index_y,
          std::size_t index_z, std::size_t index_w>
struct VectorConverter<Trait, index_x, index_y, index_z, index_w> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> VectorType
  {
    return VectorType(data[index_x], data[index_y], data[index_z],
                      data[index_w]);
  }
};

template <typename Trait> struct VectorConverter<Trait, 0, 1, 2, 3> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*bit_cast<const VectorType*>(data));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) -> VectorType&
  {
    return (*bit_cast<VectorType*>(data));
  }
};

} // namespace detail

template <typename Trait, std::size_t dimensions, std::size_t... indices>
struct Subvector {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  std::array<ValueType, dimensions> elem;

  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  [[nodiscard]] constexpr operator VectorType() noexcept
  {
    return (detail::VectorConverter<Trait, indices...>::convert(elem.data()));
  }

  // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
  [[nodiscard]] constexpr operator VectorType() const noexcept
  {
    return (detail::VectorConverter<Trait, indices...>::convert(elem.data()));
  }

  constexpr auto operator=(VectorType v) noexcept -> Subvector&
  {
    return assignment_impl(v, Assign<ValueType>{},
                           std::make_index_sequence<dimensions>{});
  }

  constexpr auto operator+=(typename Trait::VectorType v) noexcept -> Subvector&
  {
    return assignment_impl(v, PlusEqual<ValueType>{},
                           std::make_index_sequence<dimensions>{});
  }

  constexpr auto operator-=(typename Trait::VectorType v) noexcept -> Subvector&
  {
    return assignment_impl(v, MinusEqual<ValueType>{},
                           std::make_index_sequence<dimensions>{});
  }

  template <typename T>
  constexpr auto operator*=(T scalar) noexcept -> Subvector&
  {
    ((elem[indices] *= scalar), ...);
    return (*this);
  }

  template <typename T>
  constexpr auto operator/=(T scalar) noexcept -> Subvector&
  {
    ((elem[indices] /= scalar), ...);
    return (*this);
  }

  template <std::size_t... indices2>
  constexpr auto
  operator=(Subvector<Trait, dimensions, indices2...> value) noexcept
      -> Subvector&
  {
    ((elem[indices] = value.elem[indices2]), ...);
    return (*this);
  }

private:
  // Implementation helper for assignment operations
  template <typename Func, std::size_t... indices2>
  constexpr auto assignment_impl(VectorType v, Func f,
                                 std::index_sequence<indices2...>) noexcept
      -> Subvector&
  {
    (f(elem[indices], v[indices2]), ...);
    return (*this);
  }
};

template <typename Trait, std::size_t index_x, std::size_t index_y>
using Subvector2 = Subvector<Trait, 2, index_x, index_y>;

template <typename Trait, std::size_t index_x, std::size_t index_y,
          std::size_t index_z>
using Subvector3 = Subvector<Trait, 3, index_x, index_y, index_z>;

template <typename Trait, std::size_t index_x, std::size_t index_y,
          std::size_t index_z, std::size_t index_w>
using Subvector4 = Subvector<Trait, 4, index_x, index_y, index_z, index_w>;

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          std::size_t... indices2>
[[nodiscard]] constexpr auto
operator==(const Subvector<Trait, dimensions, indices1...>& v1,
           const Subvector<Trait, dimensions, indices2...>& v2) noexcept -> bool
{
  return (... && (v1.elem[indices1] == v2.elem[indices2]));
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          std::size_t... indices2>
[[nodiscard]] constexpr auto
operator!=(const Subvector<Trait, dimensions, indices1...>& v1,
           const Subvector<Trait, dimensions, indices2...>& v2) noexcept -> bool
{
  return (... || (v1.elem[indices1] != v2.elem[indices2]));
}

template <typename Trait, std::size_t dimensions, std::size_t... indices>
[[nodiscard]] constexpr auto
operator*(const Subvector<Trait, dimensions, indices...>& v,
          typename Trait::ValueType scalar) noexcept
{
  return typename Trait::template VectorTemplate<typename Trait::ValueType>{
      (v.elem[indices] * scalar)...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices,
          typename T>
[[nodiscard]] constexpr auto
operator*(T scalar, const Subvector<Trait, dimensions, indices...>& v) noexcept
{
  return v * scalar;
}

template <typename Trait, std::size_t dimensions, std::size_t... indices>
[[nodiscard]] constexpr auto
operator/(const Subvector<Trait, dimensions, indices...>& v,
          typename Trait::ValueType scalar) noexcept
{
  return typename Trait::template VectorTemplate<typename Trait::ValueType>{
      (v.elem[indices] / scalar)...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          std::size_t... indices2>
[[nodiscard]] constexpr auto
operator+(const Subvector<Trait, dimensions, indices1...>& v1,
          const Subvector<Trait, dimensions, indices2...>& v2) noexcept
{
  return typename Trait::VectorType{(v1.elem[indices1] + v2.elem[indices2])...};
}

#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
#endif

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator+(const Subvector<Trait, dimensions, indices1...>& v1,
          const typename Trait::VectorType& v2) noexcept
{
  std::size_t i = 0;
  return typename Trait::VectorType{(v1.elem[indices1] + v2.elem[i++])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator+(const typename Trait::VectorType& v2,
          const Subvector<Trait, dimensions, indices1...>& v1) noexcept
{
  std::size_t i = 0;
  return typename Trait::VectorType{(v1.elem[indices1] + v2.elem[i++])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          std::size_t... indices2>
[[nodiscard]] constexpr auto
operator-(const Subvector<Trait, dimensions, indices1...>& v1,
          const Subvector<Trait, dimensions, indices2...>& v2) noexcept
{
  return typename Trait::VectorType{(v1.elem[indices1] - v2.elem[indices2])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator-(const Subvector<Trait, dimensions, indices1...>& v1,
          const typename Trait::VectorType& v2) noexcept
{
  std::size_t i = 0;
  return typename Trait::VectorType{(v1.elem[indices1] - v2.elem[i++])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator-(const typename Trait::VectorType& v2,
          const Subvector<Trait, dimensions, indices1...>& v1) noexcept
{
  std::size_t i = 0;
  return typename Trait::VectorType{(v2.elem[i++] - v1.elem[indices1])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          std::size_t... indices2>
[[nodiscard]] constexpr auto
dot_impl(const Subvector<Trait, dimensions, indices1...>& v1,
         const typename Trait::VectorType& v2, std::index_sequence<indices2...>)
{
  return (... + (v1.elem[indices1] * v2[indices2]));
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          typename Incices = std::make_index_sequence<dimensions>>
[[nodiscard]] constexpr auto
dot(const Subvector<Trait, dimensions, indices1...>& v1,
    const typename Trait::VectorType& v2) noexcept
{
  return dot_impl(v1, v2, std::make_index_sequence<dimensions>{});
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
dot(const typename Trait::VectorType& v2,
    const Subvector<Trait, dimensions, indices1...>& v1) noexcept
{
  return dot_impl(v1, v2, std::make_index_sequence<dimensions>{});
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1,
          std::size_t... indices2>
[[nodiscard]] constexpr auto
dot(const Subvector<Trait, dimensions, indices1...>& v1,
    const Subvector<Trait, dimensions, indices2...>& v2) noexcept
{
  return (... + (v1.elem[indices1] * v2.elem[indices2]));
}

#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

template <typename Trait, std::size_t idx1, std::size_t idy1, std::size_t idz1,
          std::size_t idx2, std::size_t idy2, std::size_t idz2>
[[nodiscard]] constexpr auto
cross(const Subvector<Trait, 3, idx1, idy1, idz1>& v1,
      const Subvector<Trait, 3, idx2, idy2, idz2>& v2) noexcept
{
  return typename Trait::VectorType{
      (v1.elem[idy1] * v2.elem[idz2]) - (v1.elem[idz1] * v2.elem[idy2]),
      (v1.elem[idz1] * v2.elem[idx2]) - (v1.elem[idx1] * v2.elem[idz2]),
      (v1.elem[idx1] * v2.elem[idy2]) - (v1.elem[idy1] * v2.elem[idx2])};
}

template <typename Trait, std::size_t idx2, std::size_t idy2, std::size_t idz2>
[[nodiscard]] constexpr auto
cross(const typename Trait::VectorType& v1,
      const Subvector<Trait, 3, idx2, idy2, idz2>& v2) noexcept
{
  return typename Trait::VectorType{
      (v1.y * v2.elem[idz2]) - (v1.z * v2.elem[idy2]),
      (v1.z * v2.elem[idx2]) - (v1.x * v2.elem[idz2]),
      (v1.x * v2.elem[idy2]) - (v1.y * v2.elem[idx2])};
}

template <typename Trait, std::size_t idx1, std::size_t idy1, std::size_t idz1>
[[nodiscard]] constexpr auto
cross(const Subvector<Trait, 3, idx1, idy1, idz1>& v1,
      const typename Trait::VectorType& v2) noexcept
{
  return typename Trait::VectorType{
      (v1.elem[idy1] * v2.z) - (v1.elem[idz1] * v2.y),
      (v1.elem[idz1] * v2.x) - (v1.elem[idx1] * v2.z),
      (v1.elem[idx1] * v2.y) - (v1.elem[idy1] * v2.x)};
}

#if defined(__clang__)
_Pragma("clang diagnostic push")
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

    template <typename Derived>
    struct VectorStorage<Derived, 2> {
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
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 0, 1> xy;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 1, 0> yx;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 0, 2> xz;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 2, 0> zx;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 1, 2> yz;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 2, 1> zy;
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
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 0, 1> xy;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 1, 0> yx;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 0, 2> xz;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 2, 0> zx;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 1, 2> yz;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 2, 1> zy;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 0, 3> xw;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 1, 3> yw;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 2, 3> zw;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 3, 0> wx;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 3, 1> wy;
    Subvector2<VectorTrait<TVec<ValueType, 2>>, 3, 2> wz;

    Subvector3<VectorTrait<TVec<ValueType, 3>>, 0, 1, 2> xyz;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 0, 1, 3> xyw;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 0, 2, 1> xzy;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 0, 2, 3> xzw;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 0, 3, 1> xwy;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 0, 3, 2> xwz;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 1, 0, 2> yxz;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 1, 0, 3> yxw;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 1, 2, 0> yzx;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 1, 2, 3> yzw;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 1, 3, 0> ywx;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 1, 3, 2> ywz;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 2, 0, 1> zxy;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 2, 0, 3> zxw;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 2, 1, 0> zyx;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 2, 1, 3> zyw;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 2, 3, 0> zwx;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 2, 3, 1> zwy;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 3, 0, 1> wxy;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 3, 0, 2> wxz;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 3, 1, 0> wyx;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 3, 1, 2> wyz;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 3, 2, 0> wzx;
    Subvector3<VectorTrait<TVec<ValueType, 3>>, 3, 2, 1> wzy;

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
#endif

/**
 * @brief This class serve as base class for TVec, Bivector, and Trivectors.
 *
 * This class contains all the possible operations on vectors and anti-vectors
 * (Bivector and Trivector).
 *
 * @see TVec, VectorStorage
 */
template <typename T, std::size_t... Ns>
struct VectorBase : VectorStorage<TVec<T, sizeof...(Ns)>, sizeof...(Ns)> {
  using ValueType = T;
  using Storage = VectorStorage<TVec<T, sizeof...(Ns)>, sizeof...(Ns)>;

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
  [[nodiscard]] auto length() const noexcept
      -> std::enable_if_t<std::is_floating_point_v<U>, U>
  {
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
  constexpr auto operator/=(U rhs) noexcept
      -> std::enable_if_t<std::is_floating_point_v<U>, VectorBase&>
  {
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
    -> VectorBase<T, Ns...>
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
    -> VectorBase<T, Ns...>
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
                                       T scalar) noexcept
    -> std::enable_if_t<std::is_floating_point_v<T>, VectorBase<T, Ns...>>
{
  lhs /= scalar;
  return lhs;
}

/// @brief Normalizes a vector into a unit vector
/// @note Only available for floating point vectors
template <typename T, std::size_t... Ns>
[[nodiscard]] constexpr auto normalize(const VectorBase<T, Ns...>& v) noexcept
    -> std::enable_if_t<std::is_floating_point_v<T>, TVec<T, sizeof...(Ns)>>
{
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

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_MATH_VECTOR_HPP
