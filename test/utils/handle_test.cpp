#include <catch2/catch_test_macros.hpp>

#include <vector>

#include <beyond/utils/handle.hpp>

using namespace beyond;

namespace {

struct DummyHandle : GenerationalHandle<DummyHandle, std::uint32_t, 16> {
  using GenerationalHandle::GenerationalHandle;
};

} // anonymous namespace

TEST_CASE("Resource handle", "[beyond.core.util.handle]")
{
  STATIC_REQUIRE(sizeof(DummyHandle) == sizeof(std::uint32_t));

  const DummyHandle hd2{10};
  REQUIRE(hd2.index() == 10);
  REQUIRE(hd2.generation() == 0);

  const DummyHandle hd3{10, 10};
  REQUIRE(hd3.index() == 10);
  REQUIRE(hd3.generation() == 10);

  REQUIRE(hd2 != hd3);
}
