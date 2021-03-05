#ifndef BEYOND_CORE_UTILS_ARRAY_LENGTH_HPP
#define BEYOND_CORE_UTILS_ARRAY_LENGTH_HPP

#include "force_inline.hpp"

namespace beyond {

template <typename T, std::uint32_t size>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
    array_length(T (&/*arr*/)[size]) noexcept -> std::uint32_t
{
  return size;
}

} // namespace beyond

#endif // BEYOND_CORE_UTILS_ARRAY_LENGTH_HPP
