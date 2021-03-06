#include <catch2/catch.hpp>

#include <beyond/utils/size.hpp>

TEST_CASE("beyond::size of raw array", "[beyond.core.util.size]")
{
  int _arr[] = {1, 2, 3, 4, 5};
  STATIC_REQUIRE(beyond::size(_arr) == sizeof(_arr) / sizeof(int));
}