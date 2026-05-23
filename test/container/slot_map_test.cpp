#include <catch2/catch_test_macros.hpp>

#include <string>

import beyond.core;

using namespace beyond;

struct DymmyHandle : GenerationalHandle<DymmyHandle, std::uint32_t, 16> {
  using GenerationalHandle::GenerationalHandle;
};

TEST_CASE("SlotMap", "[beyond.core.container.slot_map]")
{
  SlotMap<DymmyHandle, std::string> map;
  REQUIRE(map.size() == 0);
  REQUIRE(map.capacity() == 0);

  const auto hello = map.emplace("Hello");
  const auto comma = map.emplace(", ");
  const auto world = map.emplace("world");

  REQUIRE(map.size() == 3);
  REQUIRE(map.try_get(hello) == "Hello");
  REQUIRE(map.try_get(comma) == ", ");
  REQUIRE(map.try_get(world) == "world");

  map.erase(hello);
  REQUIRE(map.size() == 2);
  REQUIRE(map.try_get(hello) == beyond::nullopt);
  REQUIRE(map.try_get(comma) == ", ");
  REQUIRE(map.try_get(world) == "world");

  map.erase(world);
  REQUIRE(map.size() == 1);
  REQUIRE(map.try_get(hello) == beyond::nullopt);
  REQUIRE(map.try_get(comma) == ", ");
  REQUIRE(map.try_get(world) == beyond::nullopt);

  const auto hello2 = map.emplace("Hello 2");
  REQUIRE(map.size() == 2);
  REQUIRE(map.try_get(hello) == beyond::nullopt);
  REQUIRE(map.try_get(hello2) == "Hello 2");
  REQUIRE(map.try_get(comma) == ", ");
  REQUIRE(map.try_get(world) == beyond::nullopt);

  const auto world2 = map.emplace("World 2");
  REQUIRE(map.size() == 3);
  REQUIRE(map.try_get(hello) == beyond::nullopt);
  REQUIRE(map.try_get(hello2) == "Hello 2");
  REQUIRE(map.try_get(comma) == ", ");
  REQUIRE(map.try_get(world) == beyond::nullopt);
  REQUIRE(map.try_get(world2) == "World 2");
}

TEST_CASE("SlotMap LIFO Erase", "[beyond.core.container.slot_map]")
{
  SlotMap<DymmyHandle, int> map;

  const auto first = map.emplace(42);
  const auto second = map.emplace(32);

  REQUIRE(map.size() == 2);
  REQUIRE(map.try_get(first) == 42);
  REQUIRE(map.try_get(second) == 32);

  map.erase(second);

  REQUIRE(map.size() == 1);
  REQUIRE(map.try_get(first) == 42);
  REQUIRE(map.try_get(second) == nullopt);

  map.erase(first);

  REQUIRE(map.size() == 0);
  REQUIRE(map.try_get(first) == nullopt);
  REQUIRE(map.try_get(second) == nullopt);
}
