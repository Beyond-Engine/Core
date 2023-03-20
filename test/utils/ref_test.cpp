#include <catch2/catch_test_macros.hpp>

#include "beyond/utils/ref.hpp"
#include <algorithm>
#include <vector>

constexpr auto f() -> int
{
  int x = 42;
  beyond::Ref rx{x};
  ++rx.get();
  return x;
}

TEST_CASE("Ref Test", "[beyond.core.utils.ref]")
{
  SECTION("Mutation by Ref")
  {
    constexpr auto mutate_by_ref_get = []() {
      int x = 42;
      beyond::Ref rx{x};
      ++rx.get();
      return x;
    };
    STATIC_REQUIRE(mutate_by_ref_get() == 43);
  }

  SECTION("Sort")
  {
    constexpr int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    constexpr int arr[] = {4, 8, 1, 7, 3, 2, 5, 6};
    std::vector<beyond::Ref<const int>> v{std::begin(arr), std::end(arr)};
    std::sort(v.begin(), v.end());
    REQUIRE(std::equal(v.begin(), v.end(), expected));
  }

  SECTION("ref and cref")
  {
    using beyond::cref;
    using beyond::ref;

    int x = 42;

    auto rx = ref(x);
    REQUIRE(rx == x);

    auto crx = cref(x);
    REQUIRE(crx == x);

    auto rx2 = rx;
    REQUIRE(rx2 == x);

    auto crx2 = rx;
    REQUIRE(crx2 == x);

    auto crx3 = rx2;
    REQUIRE(crx3 == x);
  }
}