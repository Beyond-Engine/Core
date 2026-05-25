module;

#include <beyond/prelude.hpp>

export module beyond.core:noexcept_cast;

namespace beyond {

template <typename Fnc> struct noexcept_cast_helper;

template <typename Ret, typename... Args>
struct noexcept_cast_helper<Ret (*)(Args...)> {
  using type = Ret (*)(Args...) noexcept;
};

/*
 * @brief Casts a function to noexcept, useful for C functions
 *
 * The idea and this implementation comes from Hana Dusíková (@hankadusikova)
 */
export template <typename T>
[[nodiscard]] BEYOND_FORCE_INLINE auto noexcept_cast(T obj) noexcept
{ return reinterpret_cast<noexcept_cast_helper<T>::type>(obj); };

} // namespace beyond
