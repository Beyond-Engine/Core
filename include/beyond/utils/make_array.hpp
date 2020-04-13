#ifndef BEYOND_CORE_UTILS_MAKE_ARRAY_HPP
#define BEYOND_CORE_UTILS_MAKE_ARRAY_HPP

#include <array>
#include <type_traits>
#include <utility>

/**
 * @file make_array.hpp
 * @ingroup util
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

/**
 * @brief Creates a `std::array` whose size is equal to the number of arguments
 * and whose elements are initialized from the corresponding arguments.
 */
template <class... Ts>
constexpr auto make_array(Ts&&... ts) -> std::array<
    typename std::decay<typename std::common_type<Ts...>::type>::type,
    sizeof...(Ts)>
{
  return std::array<
      typename std::decay<typename std::common_type<Ts...>::type>::type,
      sizeof...(Ts)>{std::forward<Ts>(ts)...};
}

/** @}@} */

} // namespace beyond

#endif // BEYOND_CORE_UTILS_MAKE_ARRAY_HPP
