#ifndef BEYOND_CORE_UTILS_BYTE_SIZE_HPP
#define BEYOND_CORE_UTILS_BYTE_SIZE_HPP

#include "force_inline.hpp"
#include <vector>

namespace beyond {

/**
 * @brief Gets the data size in bytes of a container
 */
template <typename T>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
byte_size(const std::vector<T>& v) noexcept -> std::uint32_t
{
  return static_cast<std::uint32_t>(v.size() * sizeof(T));
}

template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
    byte_size(T (&/*arr*/)[N]) noexcept -> std::uint32_t
{
  return N * sizeof(T);
}

} // namespace beyond

#endif // BEYOND_CORE_UTILS_BYTE_SIZE_HPP
