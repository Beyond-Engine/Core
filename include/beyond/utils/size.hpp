#ifndef BEYOND_CORE_UTILS_SIZE_HPP
#define BEYOND_CORE_UTILS_SIZE_HPP

#include "force_inline.hpp"

namespace beyond {

/**
 * @brief Similar to std::size, but returns an uint32_t
 */
template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto size(T (&/*arr*/)[N]) noexcept
    -> std::uint32_t
{
  return N;
}

} // namespace beyond

#endif // BEYOND_CORE_UTILS_SIZE_HPP
