#ifndef BEYOND_CORE_TYPES_OPTIONAL_CONVERSION_HPP
#define BEYOND_CORE_TYPES_OPTIONAL_CONVERSION_HPP

#include "../utils/utils.hpp"
#include "optional.hpp"
#include <optional>

/**
 * @file Provides back and forth conversion between std::optional and
 * beyond::optional
 */

namespace beyond {

[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto to_std(beyond::nullopt_t)
    -> std::nullopt_t
{
  return std::nullopt;
}

[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto from_std(std::nullopt_t)
    -> beyond::nullopt_t
{
  return beyond::nullopt;
}

template <typename T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto to_std(beyond::optional<T> opt)
    -> std::optional<T>
{
  if (opt.has_value()) { return std::optional<T>{opt.value()}; }
  return std::nullopt;
}

template <typename T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto from_std(std::optional<T> opt)
    -> beyond::optional<T>
{
  if (opt.has_value()) { return beyond::optional<T>{opt.value()}; }
  return beyond::nullopt;
}

} // namespace beyond

#endif // BEYOND_CORE_TYPES_OPTIONAL_CONVERSION_HPP
