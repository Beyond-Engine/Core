#ifndef BEYOND_CORE_UTILS_NARROWING_HPP
#define BEYOND_CORE_UTILS_NARROWING_HPP

#include "utils.hpp"

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
    : public std::integral_constant<bool, std::is_signed<T>::value ==
                                              std::is_signed<U>::value> {};
} // namespace details

/// @brief Checked narrowing conversion
template <class T, class U> BEYOND_FORCE_INLINE auto narrow(U u) -> T
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

#endif // BEYOND_CORE_UTILS_NARROWING_HPP
