module;

#include <functional>

export module beyond.core:hash;

import :typedefs;

export namespace beyond {

template <typename T, typename... Ts>
[[nodiscard]] auto hash_combine(T first, Ts... rest) -> usize
{
  std::size_t seed = std::hash<T>{}(first);
  ((seed ^= std::hash<Ts>{}(rest) + 0x9e3779b9u + (seed << 6u) + (seed >> 2u)),
   ...);
  return seed;
}

} // namespace beyond
