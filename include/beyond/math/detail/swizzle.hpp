#ifndef BEYOND_CORE_MATH_SWIZZLE_HPP
#define BEYOND_CORE_MATH_SWIZZLE_HPP

#include <array>
#include <cmath>
#include <type_traits>
#include <utility>

#include "../../utils/functional.hpp"

namespace beyond {

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
    return (*std::bit_cast<const VectorType*>(data));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) noexcept
      -> VectorType&
  {
    return (*std::bit_cast<VectorType*>(data));
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

  [[nodiscard]] static constexpr auto convert(ValueType* data) noexcept
      -> VectorType&
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

  [[nodiscard]] static constexpr auto convert(ValueType* data) noexcept
      -> VectorType&
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
    return (*std::bit_cast<const VectorType*>(data));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) noexcept
      -> VectorType&
  {
    return (*std::bit_cast<VectorType*>(data));
  }
};

template <typename Trait> struct VectorConverter<Trait, 1, 2, 3> {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;

  [[nodiscard]] static constexpr auto convert(const ValueType* data) noexcept
      -> const VectorType&
  {
    return (*std::bit_cast<const VectorType*>(data + 1));
  }

  [[nodiscard]] static constexpr auto convert(ValueType* data) noexcept
      -> VectorType&
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

  [[nodiscard]] static constexpr auto convert(ValueType* data) noexcept
      -> VectorType&
  {
    return (*bit_cast<VectorType*>(data));
  }
};

} // namespace detail

template <typename Trait, std::size_t dimensions, std::size_t... indices>
struct Subvector {
  using ValueType = typename Trait::ValueType;
  using VectorType = typename Trait::VectorType;
  using PointType = typename Trait::PointType;

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

  [[nodiscard]] constexpr operator PointType() noexcept
  {
    return std::bit_cast<PointType>(
        detail::VectorConverter<Trait, indices...>::convert(elem.data()));
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
operator+(const Subvector<Trait, dimensions, indices1...>& v1,
          const typename Trait::PointType& v2) noexcept
{
  std::size_t i = 0;
  return typename Trait::PointType{(v1.elem[indices1] + v2.elem[i++])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator+(const typename Trait::VectorType& v2,
          const Subvector<Trait, dimensions, indices1...>& v1) noexcept
{
  std::size_t i = 0;
  return typename Trait::VectorType{(v1.elem[indices1] + v2.elem[i++])...};
}

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator+(const typename Trait::PointType& v2,
          const Subvector<Trait, dimensions, indices1...>& v1) noexcept
{
  std::size_t i = 0;
  return typename Trait::PointType{(v1.elem[indices1] + v2.elem[i++])...};
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

template <typename Trait, std::size_t dimensions, std::size_t... indices1>
[[nodiscard]] constexpr auto
operator-(const typename Trait::PointType& v2,
          const Subvector<Trait, dimensions, indices1...>& v1) noexcept
{
  std::size_t i = 0;
  return typename Trait::PointType{(v2.elem[i++] - v1.elem[indices1])...};
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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4201)
#endif

} // namespace beyond

#endif // BEYOND_CORE_MATH_SWIZZLE_HPP
