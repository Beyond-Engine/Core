module;

#include <beyond/prelude.hpp>

#include <type_traits>

export module beyond.core:narrowing;

import :panic;

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

namespace details {
template <class T, class U>
struct is_same_signedness
    : std::integral_constant<bool, std::is_signed_v<T> == std::is_signed_v<U>> {
};
} // namespace details

/// @brief Checked narrowing conversion
export template <class T, class U> BEYOND_FORCE_INLINE auto narrow(U u) -> T
{
  T t = static_cast<T>(u);
  if (static_cast<U>(t) != u) beyond::panic("Narrowing error");
  if constexpr (!details::is_same_signedness<T, U>::value) {
    if ((t < T{}) != (u < U{})) { beyond::panic("Narrowing error"); }
  }

  return t;
}

/** @}@} */

} // namespace beyond
