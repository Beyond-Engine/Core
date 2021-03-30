#include <cstdlib>

#include <fmt/format.h>

#if __has_include(<backward.hpp>)

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)
#endif

#include <backward.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

#include "beyond/utils/panic.hpp"

namespace beyond {

[[noreturn]] auto panic(std::string_view msg) noexcept -> void
{
  fmt::print(stderr, "beyond panic() invoked with message:\n{}\n", msg);

#if __has_include(<backward.hpp>)
  backward::StackTrace st;
  st.load_here(32);
  backward::Printer p;
  p.print(st);
#endif

  std::abort();
}

} // namespace beyond
