module;

/**
 * @file utils.cppm
 * @brief Misc utility functions
 */

#include <cstdint>
#include <ranges>

#include <beyond/prelude.hpp>

export module beyond.core:utils;

import :typedefs;

export namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

/**
 * @brief Similar to std::size, but returns an uint32_t
 */
template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto size(T (& /*arr*/)[N]) noexcept
    -> std::uint32_t
{ return N; }

/**
 * @brief Suppresses the implicit-decay-array warning
 */
template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
to_pointer(T (&arr)[N]) noexcept -> T*
{ return static_cast<T*>(arr); }

/**
 * @brief Gets the data size in bytes of a contiguous range
 */
template <std::ranges::contiguous_range Range>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto byte_size(Range v) noexcept
    -> u32
{
  return static_cast<std::uint32_t>(v.size() *
                                    sizeof(std::ranges::range_value_t<Range>));
}

template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
byte_size(T (& /*arr*/)[N]) noexcept -> u32
{ return N * sizeof(T); }

/** @}@} */

} // namespace beyond
