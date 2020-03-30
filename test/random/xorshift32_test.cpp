#include <catch2/catch.hpp>

#include "beyond/random/generators/xorshift32.hpp"
#include <limits>
#include <random>

TEST_CASE("xorshift32", "[beyond.random]")
{
  STATIC_REQUIRE(beyond::xorshift32::min() ==
                 std::numeric_limits<std::uint32_t>::min());
  STATIC_REQUIRE(beyond::xorshift32::max() ==
                 std::numeric_limits<std::uint32_t>::max());

  SECTION("Default constructor")
  {
    beyond::xorshift32 g;

    REQUIRE(g.state == 1u);
    REQUIRE(g() == 268476417u);
    REQUIRE(g() == 1157628417u);
  }

  SECTION("Generated by std::random device")
  {
    beyond::xorshift32 g{std::random_device{}()};
  }
}
