#ifndef BEYOND_CORE_UTILS_NOEXCEPT_CAST_HPP
#define BEYOND_CORE_UTILS_NOEXCEPT_CAST_HPP

#include "force_inline.hpp"

namespace beyond {

namespace details {

template <typename Fnc> struct noexcept_cast_helper;

template <typename Ret, typename... Args>
struct noexcept_cast_helper<Ret (*)(Args...)> {
  using type = Ret (*)(Args...) noexcept;
};

} // namespace details

/*
 * @brief Casts a function to noexcept, useful for C functions
 *
 * The idea and this implementation comes from Hana Dusíková (@hankadusikova)
 */
template <typename T>
[[nodiscard]] BEYOND_FORCE_INLINE auto noexcept_cast(T obj) noexcept
{
  return reinterpret_cast<typename details::noexcept_cast_helper<T>::type>(obj);
};

} // namespace beyond

#endif // BEYOND_CORE_UTILS_NOEXCEPT_CAST_HPP
