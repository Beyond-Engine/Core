#include <cstdlib>

#include <fmt/format.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)
#endif

#include <backward.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

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
