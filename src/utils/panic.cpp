#include <cstdlib>
#include <iostream>

#include <fmt/format.h>

#include <backward.hpp>

#include "beyond/utils/panic.hpp"

namespace beyond {

[[noreturn]] auto panic(std::string_view msg) noexcept -> void
{
  backward::StackTrace st;
  st.load_here(32);
  backward::Printer p;
  p.print(st);

  fmt::print("beyond panic() invoked with message:\n{}", msg);
  std::abort();
}

} // namespace beyond
