#include <cstdlib>
#include <iostream>

#include "beyond/utils/panic.hpp"

namespace beyond {

[[noreturn]] auto panic(std::string_view msg) noexcept -> void
{
  std::cerr << msg.data();
  std::abort();
}

} // namespace beyond
