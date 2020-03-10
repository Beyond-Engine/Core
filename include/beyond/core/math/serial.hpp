#pragma once

#ifndef BEYOND_CORE_MATH_SERIAL_HPP
#define BEYOND_CORE_MATH_SERIAL_HPP

/**
 * @file serial.hpp
 * @brief Provides Serialization codes for the mathmatic types
 * @ingroup math
 */

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <sstream>
#include <string>

#include "angle.hpp"
#include "matrix.hpp"
#include "vector.hpp"

namespace beyond {

template <typename T, std::size_t n>
[[nodiscard]] auto to_string(const Vector<T, n>& v) -> std::string
{
  return fmt::format("vec({})", fmt::join(v.elem, ", "));
}

template <typename T, std::size_t n>
auto operator<<(std::ostream& os, const Vector<T, n>& v) -> std::ostream&
{
  os << beyond::to_string(v);
  return os;
}

template <typename T, std::size_t n>
[[nodiscard]] auto to_string(const Point<T, n>& v) -> std::string
{
  return fmt::format("point({})", fmt::join(v.elem, ", "));
}

template <typename T, std::size_t n>
auto operator<<(std::ostream& os, const Point<T, n>& v) -> std::ostream&
{
  os << beyond::to_string(v);
  return os;
}

template <typename T>[[nodiscard]] auto to_string(Degree<T> r) -> std::string
{
  return fmt::format("{}_degree", r.value());
}

template <typename T>[[nodiscard]] auto to_string(Radian<T> r) -> std::string
{
  return fmt::format("{}_radian", r.value());
}

/// @brief Prints the Degree
/// @related Degree
template <typename T>
auto operator<<(std::ostream& os, Degree<T> d) -> std::ostream&
{
  os << beyond::to_string(d);
  return os;
}

/// @brief Prints the Radian
/// @related Degree
template <typename T>
auto operator<<(std::ostream& os, Radian<T> r) -> std::ostream&
{
  os << beyond::to_string(r);
  return os;
}

template <typename Derived>
auto operator<<(std::ostream& os, const MatrixBase<Derived>& m) -> std::ostream&
{
  os << "mat" << m.dimension() << "(\n";
  for (std::size_t i = 0; i < m.dimension(); i++) {
    for (std::size_t j = 0; j < m.dimension() - 1; j++) {
      os << m.underlying().data[j * m.dimension() + i] << ", ";
    }
    os << m.underlying().data[(m.dimension() - 1) * m.dimension() + i] << ",\n";
  }
  os << ')';
  return os;
}

template <typename Derived>
[[nodiscard]] auto to_string(const MatrixBase<Derived>& m) -> std::string
{
  std::stringstream ss;
  ss << m;
  return ss.str();
}

} // namespace beyond

#endif // BEYOND_CORE_MATH_SERIAL_HPP
