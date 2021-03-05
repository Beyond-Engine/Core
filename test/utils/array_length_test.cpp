#include <catch2/catch.hpp>

#include <beyond/utils/array_length.hpp>

TEST_CASE("array size test", "[beyond.core.util.array_length]")
{
  int _arr[] = {1, 2, 3, 4, 5};
  STATIC_REQUIRE(beyond::array_length(_arr) == sizeof(_arr) / sizeof(int));
}