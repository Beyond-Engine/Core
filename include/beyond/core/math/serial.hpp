#pragma once

#ifndef BEYOND_CORE_MATH_SERIAL_HPP
#define BEYOND_CORE_MATH_SERIAL_HPP

/**
 * @file serial.hpp
 * @brief Provides Serialization codes for the mathmatic types
 * @ingroup math
 */

#include <iosfwd>
#include <ostream>
#include <string>

#include "angle.hpp"
#include "matrix.hpp"
#include "vector.hpp"

namespace beyond {

template <typename T, std::size_t n>
[[nodiscard]] extern auto to_string(const Vector<T, n>& v) -> std::string;

template <typename T, std::size_t n>
auto operator<<(std::ostream& os, const Vector<T, n>& v) -> std::ostream&
{
  os << beyond::to_string(v);
  return os;
}

template <typename T, std::size_t n>
[[nodiscard]] extern auto to_string(const Point<T, n>& v) -> std::string;

template <typename T, std::size_t n>
auto operator<<(std::ostream& os, const Point<T, n>& v) -> std::ostream&
{
  os << beyond::to_string(v);
  return os;
}

template <typename T>
[[nodiscard]] extern auto to_string(Degree<T> d) -> std::string;

template <typename T>
[[nodiscard]] extern auto to_string(Radian<T> r) -> std::string;

/// @brief Prints the Degree
/// @related Degree
template <typename T>
extern auto operator<<(std::ostream& os, Degree<T> d) -> std::ostream&;

/// @brief Prints the Radian
/// @related Degree
template <typename T>
extern auto operator<<(std::ostream& os, Radian<T> r) -> std::ostream&;

} // namespace beyond

#endif // BEYOND_CORE_MATH_SERIAL_HPP
