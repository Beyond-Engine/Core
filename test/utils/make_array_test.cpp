#include <catch2/catch.hpp>

#include <string>

#include "beyond/utils/make_array.hpp"

TEST_CASE("make_array", "[beyond.core.utils.make_array]")
{
  auto result = beyond::make_array<std::string>("1", "2", "3", "4", "5");
  REQUIRE(result.size() == 5);
  REQUIRE(result == std::array<std::string, 5>{"1", "2", "3", "4", "5"});
}