#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>

#include <fmt/core.h>

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

#if defined(WIN32) && !defined(NDEBUG)
#include <debugapi.h>
#endif

#include "beyond/utils/panic.hpp"

namespace beyond {

[[noreturn]] void panic(std::string_view msg,
                        std::source_location source_location)
{
  std::string panic_message;
  fmt::format_to(std::back_inserter(panic_message),
                 "Thread panicked at: {}:{}:{}:\n", source_location.file_name(),
                 source_location.line(), source_location.column());
  fmt::format_to(std::back_inserter(panic_message), "{}\n", msg);

  const char* use_backtrace = std::getenv("BEYOND_BACKTRACE");

  fmt::println(stderr, "{}", panic_message);

#if __has_include(<backward.hpp>)
  if (use_backtrace == nullptr or strcmp(use_backtrace, "1") != 0) {

    fmt::println(stderr,
                 "note: run with `BEYOND_BACKTRACE=1` environment variable to "
                 "display a backtrace");

  } else {
    backward::StackTrace st;
    st.load_here(32);

    // Skip backward-cpp implementation details
    st.skip_n_firsts(2);

    backward::Printer p;
    p.print(st, stderr);
  }
#endif

#if defined(WIN32) && !defined(NDEBUG)
  DebugBreak();
#endif

  std::fflush(stderr);
  std::abort();
}

} // namespace beyond
