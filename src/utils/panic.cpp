#include <cstdio>
#include <cstdlib>

#include "beyond/utils/panic.hpp"

namespace beyond {

[[noreturn]] auto panic(std::string_view msg) noexcept -> void
{
  std::fputs(msg.data(), stderr);
  std::fflush(stderr);
  std::abort();
}

} // namespace beyond
