#include <catch2/catch_test_macros.hpp>

#include "beyond/coroutine/generator.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>

static auto range(int low, int high) -> beyond::Generator<int>
{
  for (int i = low; i < high; ++i) {
    co_yield i;
  }
}

TEST_CASE("Generator basics", "[beyond.core.coroutine.generator]")
{
  auto gen = range(0, 3);
  auto itr = gen.begin();
  REQUIRE(*itr == 0);

  ++itr;
  REQUIRE(*itr == 1);
  ++itr;
  REQUIRE(*itr == 2);
  ++itr;
  REQUIRE(itr == gen.end());
}

static auto iota(int n = 0) -> beyond::Generator<int>
{
  while (true)
    co_yield n++;
}

static_assert(std::ranges::viewable_range<beyond::Generator<int>>);

TEST_CASE("Generator with ranges", "[beyond.core.coroutine.generator]")
{
  int i = 42;
  for (int x : iota(i) | std::views::take(3)) {
    REQUIRE(i++ == x);
  }
}

TEST_CASE("Generator range algorithm test", "[beyond.core.coroutine.generator]")
{
  REQUIRE(std::ranges::equal(range(0, 3), iota(0) | std::views::take(3)));
}

static auto range_exp(int low, int high) -> beyond::Generator<int>
{
  for (int i = low; i < high; ++i) {
    co_yield i;
  }
  throw std::runtime_error{"Generator Throw"};
}

TEST_CASE("Generator that throw", "[beyond.core.coroutine.generator]")
{
  auto gen = range_exp(0, 2);
  auto itr = gen.begin();
  REQUIRE(*itr == 0);
  ++itr;
  REQUIRE(*itr == 1);
  REQUIRE_THROWS(++itr);
  REQUIRE(itr == gen.end());
}