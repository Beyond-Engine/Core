module;

/**
 * @file type_traits.cppm
 * @brief A complementary set of type traits to the standard \<type_traits\>
 * @ingroup util
 */

#include <type_traits>

export module beyond.core:type_traits;

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

template <class T, std::size_t = sizeof(T)> std::true_type is_complete_impl(T*);
std::false_type is_complete_impl(...);

/**
 * @brief Test if the type is a complete type
 * @tparam T The type to test
 * @return True if the type T is complete, false if it is incomplete type
 */
export template <class T>
using is_complete = decltype(is_complete_impl(std::declval<T*>()));

/** @} @} */

} // namespace beyond
