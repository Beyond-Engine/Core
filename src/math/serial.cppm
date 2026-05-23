module;

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <sstream>
#include <string>

export module beyond.core:math.serial;

import :math.angle;
import :math.matrix;
import :math.point;
import :math.quat;
import :math.vector;

/**
 * @file serial.hpp
 * @brief Provides Serialization codes for the mathmatic types
 * @ingroup math
 */

export namespace beyond {

template <typename T, std::size_t n>
[[nodiscard]] auto to_string(const TVec<T, n>& v) -> std::string
{
  return fmt::format("vec({})", fmt::join(v.elem, ", "));
}

template <typename T, std::size_t n>
auto operator<<(std::ostream& os, const TVec<T, n>& v) -> std::ostream&
{
  os << beyond::to_string(v);
  return os;
}

template <typename T, std::size_t n>
[[nodiscard]] auto to_string(const TPoint<T, n>& v) -> std::string
{
  return fmt::format("point({})", fmt::join(v.elem, ", "));
}

template <typename T, std::size_t n>
auto operator<<(std::ostream& os, const TPoint<T, n>& v) -> std::ostream&
{
  os << beyond::to_string(v);
  return os;
}

template <typename T>[[nodiscard]] auto to_string(TDegree<T> r) -> std::string
{
  return fmt::format("{}_degree", r.value());
}

template <typename T>[[nodiscard]] auto to_string(TRadian<T> r) -> std::string
{
  return fmt::format("{}_radian", r.value());
}

/// @brief Prints the TDegree
/// @related TDegree
template <typename T>
auto operator<<(std::ostream& os, TDegree<T> d) -> std::ostream&
{
  os << beyond::to_string(d);
  return os;
}

/// @brief Prints the TRadian
/// @related TDegree
template <typename T>
auto operator<<(std::ostream& os, TRadian<T> r) -> std::ostream&
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

template <typename T>
[[nodiscard]] auto to_string(const TQuat<T>& quat) noexcept -> std::string
{
  return fmt::format("quat({}, ({}, {}, {}))", quat.w, quat.x, quat.y, quat.z);
}

template <typename T>
auto operator<<(std::ostream& os, const TQuat<T>& quat) noexcept
    -> std::ostream&
{
  os << to_string(quat);
  return os;
}

} // namespace beyond
