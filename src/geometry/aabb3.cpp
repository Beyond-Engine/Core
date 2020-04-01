#include <ostream>

#include <fmt/format.h>

#include "beyond/geometry/aabb3.hpp"
#include "beyond/math/serial.hpp"

namespace beyond {

[[nodiscard]] auto to_string(const AABB3& box) -> std::string
{
  return fmt::format("AABB3(min: {}, max: {})", to_string(box.min()),
                     to_string(box.max()));
}

auto operator<<(std::ostream& os, const AABB3& box) -> std::ostream&
{
  os << to_string(box);
  return os;
}

} // namespace beyond