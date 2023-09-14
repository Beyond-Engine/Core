#pragma once

#ifndef BEYOND_CORE_UTILS_HANLDE_HPP
#define BEYOND_CORE_UTILS_HANLDE_HPP

#include <cstdint>
#include <type_traits>

#include "assert.hpp"

/**
 * @file handle.hpp
 * @brief Provides helper class for `operator->` for proxy iterators of
 * container
 * @ingroup util
 */

namespace beyond {

template <std::size_t bit_count> consteval auto minimum_uint_type_impl()
{
  if constexpr (bit_count <= 8) {
    return std::type_identity<std::uint8_t>{};
  } else if constexpr (bit_count <= 16) {
    return std::type_identity<std::uint16_t>{};
  } else if constexpr (bit_count <= 32) {
    return std::type_identity<std::uint32_t>{};
  } else if constexpr (bit_count <= 64) {
    return std::type_identity<std::uint64_t>{};
  } else {
    static_assert([]() { return false; }());
  }
}

template <std::size_t bit_count>
using minimum_uint_type =
    typename decltype(minimum_uint_type_impl<bit_count>())::type;

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

struct HandleBase {};

/**
 * @brief Template for the base class of a generational resource handle
 *
 * Handles act as none-owning references to a resource. It has
 * additional functionality of storing a generation number to check collision.
 */
template <typename Derived, typename StorageT, std::size_t index_bits>
struct GenerationalHandle : HandleBase {
public:
  using Storage = StorageT;
  static constexpr auto generation_bits = 8 * sizeof(Storage) - index_bits;

  using Index = minimum_uint_type<index_bits>;
  using Generation = minimum_uint_type<generation_bits>;

  /// @brief The shift of index bits
  static constexpr std::size_t shift = index_bits;
  static constexpr StorageT index_mask = ~(~Storage{0} >> shift << shift);

  static_assert(std::is_unsigned_v<Storage>,
                "The storage must an unsigned integer");
  static_assert(generation_bits, "Too less bits for generation");

  explicit constexpr GenerationalHandle(Index id = 0, Generation gen = 0)
      : data_{static_cast<StorageT>(id + static_cast<StorageT>(gen << shift))}
  {
    BEYOND_ENSURE(not is_overflow(id));
  }

  /// @brief Return true if the index overflows the index range
  [[nodiscard]] static constexpr auto is_overflow(Index index) -> bool
  {
    return (static_cast<StorageT>(index) >> shift) != 0;
  }

  void set_index(Index new_index)
  {
    BEYOND_ENSURE(not is_overflow(new_index));
    data_ = new_index + generation() << shift;
  }

  [[nodiscard]] auto index() const -> Index
  {
    return data_ & index_mask;
  }

  [[nodiscard]] auto generation() const -> Generation
  {
    return data_ >> shift;
  }

  [[nodiscard]] friend constexpr auto operator==(Derived lhs, Derived rhs)
      -> bool
  {
    return lhs.data_ == rhs.data_;
  }

private:
  Storage data_;
};

/** @} @} */

} // namespace beyond

#endif // BEYOND_CORE_UTILS_HANLDE_HPP
