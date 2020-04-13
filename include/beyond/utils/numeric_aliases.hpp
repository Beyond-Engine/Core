#ifndef BEYOND_CORE_UTILS_NUMERIC_ALIASES_HPP
#define BEYOND_CORE_UTILS_NUMERIC_ALIASES_HPP

#include <cstddef>
#include <cstdint>

/**
 * @file numeric_aliases.hpp
 * @ingroup util
 * @brief Rust-style number type aliases
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;
using isize = std::ptrdiff_t;

/** @}@} */

} // namespace beyond

#endif // BEYOND_CORE_UTILS_NUMERIC_ALIASES_HPP
