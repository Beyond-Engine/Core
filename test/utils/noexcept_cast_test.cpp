#include <catch2/catch.hpp>
#include <type_traits>

#include "beyond/utils/noexcept_cast.hpp"

namespace {
auto foo() -> int
{
  return 42;
}

} // anonymous namespace

TEST_CASE("noexcept_cast", "[beyond.core.utils.noexcept_cast]")
{
  STATIC_REQUIRE(!std::is_nothrow_invocable_v<decltype(foo)>);
  STATIC_REQUIRE(
      std::is_nothrow_invocable_v<decltype(beyond::noexcept_cast(foo))>);

  REQUIRE(beyond::noexcept_cast(foo)() == 42);
}