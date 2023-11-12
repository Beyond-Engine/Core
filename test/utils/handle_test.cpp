#include <catch2/catch_test_macros.hpp>

#include <vector>

#include <beyond/utils/handle.hpp>
#include <beyond/utils/utils.hpp>

using namespace beyond;

namespace {

struct DummyHandle : Handle<DummyHandle, u32> {
  using Handle::Handle;
};

struct DummyGenerationalHandle
    : GenerationalHandle<DummyGenerationalHandle, std::uint32_t, 16> {
  using GenerationalHandle::GenerationalHandle;
};

} // anonymous namespace

TEST_CASE("Handle", "[beyond.core.util.handle]")
{
  STATIC_REQUIRE(sizeof(DummyHandle) == sizeof(u32));

  const DummyHandle hd{42};
  REQUIRE(hd.value() == 42);
  REQUIRE(std::hash<DummyHandle>{}(hd) == std::hash<u32>{}(42));
}

TEST_CASE("Generational handle", "[beyond.core.util.handle]")
{
  STATIC_REQUIRE(sizeof(DummyGenerationalHandle) == sizeof(u32));

  const DummyGenerationalHandle hd2{10};
  REQUIRE(hd2.index() == 10);
  REQUIRE(hd2.generation() == 0);
  REQUIRE(std::hash<DummyGenerationalHandle>{}(hd2) == std::hash<u32>{}(10));

  const DummyGenerationalHandle hd3{10, 10};
  REQUIRE(hd3.index() == 10);
  REQUIRE(hd3.generation() == 10);

  REQUIRE(hd2 != hd3);
}
