#ifndef BEYOND_CORE_UTILS_TO_POINTER_HPP
#define BEYOND_CORE_UTILS_TO_POINTER_HPP

namespace beyond {

/**
 * @brief Suppresses the implicit-decay-array warning
 */
template <typename T, std::uint32_t N>
[[nodiscard]] BEYOND_FORCE_INLINE constexpr auto
    to_pointer(T (&arr)[N]) noexcept -> T*
{
  return static_cast<T*>(arr);
}

} // namespace beyond

#endif // BEYOND_CORE_UTILS_TO_POINTER_HPP
