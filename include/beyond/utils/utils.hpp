#ifndef BEYOND_CORE_UTILS_HPP
#define BEYOND_CORE_UTILS_HPP

#ifdef DOXYGEN_SHOULD_SKIP_THIS
#define BEYOND_FORCE_INLINE
#elif _MSC_VER
#define BEYOND_FORCE_INLINE __forceinline
#else
#define BEYOND_FORCE_INLINE inline __attribute__((always_inline))
#endif

#define BEYOND_MOV(...)                                                        \
  static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define BEYOND_FWD(...) static_cast<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace beyond {

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
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto size(T (&/*arr*/)[N]) noexcept
    -> std::uint32_t
{
  return N;
}

/**
 * @brief Suppresses the implicit-decay-array warning
 */
template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
    to_pointer(T (&arr)[N]) noexcept -> T*
{
  return static_cast<T*>(arr);
}

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

/** @}@} */

} // namespace beyond

#endif // BEYOND_CORE_UTILS_HPP
