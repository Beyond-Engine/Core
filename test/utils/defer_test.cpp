#include <catch2/catch.hpp>

#include "beyond/utils/defer.hpp"

TEST_CASE("Defer")
{
  int i = 42;
  {
    BEYOND_DEFER(--i);
    REQUIRE(i == 42);
  }
  REQUIRE(i == 41);
}