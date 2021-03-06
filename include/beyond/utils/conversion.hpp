#ifndef BEYOND_CORE_UTILS_CONVERSION_HPP
#define BEYOND_CORE_UTILS_CONVERSION_HPP

#include <cstdint>

#include "force_inline.hpp"

namespace beyond {

constexpr BEYOND_FORCE_INLINE auto to_i32(float value) noexcept
{
  return static_cast<int>(value);
}

constexpr BEYOND_FORCE_INLINE auto to_f32(int value) noexcept
{
  return static_cast<float>(value);
}

} // namespace beyond

#endif // BEYOND_CORE_UTILS_CONVERSION_HPP
