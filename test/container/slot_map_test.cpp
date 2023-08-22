#include <catch2/catch_test_macros.hpp>

#include "beyond/container/slot_map.hpp"

#include <string>

using namespace beyond;

TEST_CASE("SlotMap", "[beyond.core.container.slot_map]")
{
  struct Handle : GenerationalHandle<Handle, std::uint32_t, 16> {
    using GenerationalHandle::GenerationalHandle;
  };

  SlotMap<Handle, std::string> map;
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