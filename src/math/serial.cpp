#include <fmt/ranges.h>
#include <ostream>

#include "beyond/core/math/serial.hpp"

namespace beyond {

template <typename T, std::size_t n>
[[nodiscard]] auto to_string(const Vector<T, n>& v) -> std::string
{
  return fmt::format("vec({})", fmt::join(v.elem, ", "));
}

template <typename T, std::size_t n>
[[nodiscard]] auto to_string(const Point<T, n>& v) -> std::string
{
  return fmt::format("point({})", fmt::join(v.elem, ", "));
}

template auto to_string(const Vector<int, 2>& v) -> std::string;
template auto to_string(const Vector<int, 3>& v) -> std::string;
template auto to_string(const Vector<int, 4>& v) -> std::string;
template auto to_string(const Vector<float, 2>& v) -> std::string;
template auto to_string(const Vector<float, 3>& v) -> std::string;
template auto to_string(const Vector<float, 4>& v) -> std::string;
template auto to_string(const Vector<double, 2>& v) -> std::string;
template auto to_string(const Vector<double, 3>& v) -> std::string;
template auto to_string(const Vector<double, 4>& v) -> std::string;

template auto to_string(const Point<int, 2>& v) -> std::string;
template auto to_string(const Point<int, 3>& v) -> std::string;
template auto to_string(const Point<float, 2>& v) -> std::string;
template auto to_string(const Point<float, 3>& v) -> std::string;
template auto to_string(const Point<double, 2>& v) -> std::string;
template auto to_string(const Point<double, 3>& v) -> std::string;

template <typename T>[[nodiscard]] auto to_string(Degree<T> r) -> std::string
{
  return fmt::format("{}_degree", r.value());
}
template auto to_string(Degree<float> r) -> std::string;
template auto to_string(Degree<double> r) -> std::string;

template <typename T>[[nodiscard]] auto to_string(Radian<T> r) -> std::string
{
  return fmt::format("{}_radian", r.value());
}
template auto to_string(Radian<float> r) -> std::string;
template auto to_string(Radian<double> r) -> std::string;

/// @brief Prints the Degree
/// @related Degree
template <typename T>
auto operator<<(std::ostream& os, Degree<T> d) -> std::ostream&
{
  os << beyond::to_string(d);
  return os;
}

template auto operator<<(std::ostream& os, Degree<float> r) -> std::ostream&;
template auto operator<<(std::ostream& os, Degree<double> r) -> std::ostream&;

template <typename T>
auto operator<<(std::ostream& os, Radian<T> r) -> std::ostream&
{
  os << beyond::to_string(r);
  return os;
}

template auto operator<<(std::ostream& os, Radian<float> r) -> std::ostream&;
template auto operator<<(std::ostream& os, Radian<double> r) -> std::ostream&;

} // namespace beyond