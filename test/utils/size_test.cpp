#include <catch2/catch_test_macros.hpp>

#include <span>
#include <vector>

#include <beyond/utils/utils.hpp>

import beyond.core;

TEST_CASE("beyond::size of raw array", "[beyond.core.util.size]")
{
  int arr[] = {1, 2, 3, 4, 5};
  STATIC_REQUIRE(beyond::size(arr) == sizeof(arr) / sizeof(int));
}

TEST_CASE("beyond::byte_size of raw array", "[beyond.core.util.size]")
{
  int arr[] = {1, 2, 3, 4, 5};
  STATIC_REQUIRE(beyond::byte_size(arr) == sizeof(arr));
}

TEST_CASE("beyond::byte_size of std::vector", "[beyond.core.util.size]")
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  REQUIRE(beyond::byte_size(v) == v.size() * sizeof(int));
}

TEST_CASE("beyond::byte_size of StaticVector", "[beyond.core.util.size]")
{
  beyond::StaticVector<int, 5> v = {1, 2, 3, 4, 5};
  REQUIRE(beyond::byte_size(v) == v.size() * sizeof(int));
}

TEST_CASE("beyond::byte_size of std::span", "[beyond.core.util.size]")
{
  int arr[] = {1, 2, 3, 4, 5};
  std::span<int> s = arr;
  REQUIRE(beyond::byte_size(s) == sizeof(arr));
}
