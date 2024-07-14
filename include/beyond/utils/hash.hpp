#ifndef BEYOND_CORE_UTILS_HASH_HPP
#define BEYOND_CORE_UTILS_HASH_HPP

#include <concepts>

#include "utils.hpp"

namespace beyond {

template <typename T, typename... Ts>
[[nodiscard]] inline auto hash_combine(T first, Ts... rest) -> usize
{
  std::size_t seed = std::hash<T>{}(first);
  ((seed ^= std::hash<Ts>{}(rest) + 0x9e3779b9u + (seed << 6u) + (seed >> 2u)),
   ...);
  return seed;
}

} // namespace beyond

#endif // BEYOND_CORE_UTILS_HASH_HPP
