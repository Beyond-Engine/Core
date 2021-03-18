#ifndef BEYOND_CORE_CONTAINER_AT_OPT_HPP
#define BEYOND_CORE_CONTAINER_AT_OPT_HPP

#include <beyond/types/optional.hpp>

namespace beyond {

/**
 * @brief Like `operator[]` but returns an optional reference
 *
 * Try to random-access an index from a sequential container with bound
 * checking. If the bound checking fails, returns an `nullopt`.
 */
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto at_opt(const T (&c)[N], std::size_t index)
    -> beyond::optional<const T&>
{
  if (index >= N) { return beyond::nullopt; }
  return c[index];
}

/// @overload
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto at_opt(T (&c)[N], std::size_t index)
    -> beyond::optional<T&>
{
  if (index >= N) { return beyond::nullopt; }
  return c[index];
}

/// @overload
template <typename Container>
[[nodiscard]] constexpr auto at_opt(Container& c,
                                    typename Container::size_type index)
    -> beyond::optional<typename Container::value_type&>
{
  if (index >= c.size()) { return beyond::nullopt; }
  return c[index];
}

/// @overload
template <typename Container>
[[nodiscard]] constexpr auto at_opt(const Container& c,
                                    typename Container::size_type index)
    -> beyond::optional<const typename Container::value_type&>
{
  if (index >= c.size()) { return beyond::nullopt; }
  return c[index];
}

} // namespace beyond

#endif // BEYOND_CORE_CONTAINER_AT_OPT_HPP
