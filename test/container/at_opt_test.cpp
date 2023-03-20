#include <catch2/catch_test_macros.hpp>

#include <beyond/container/at_opt.hpp>

#include <array>
#include <beyond/container/static_vector.hpp>
#include <vector>

TEST_CASE("at_opt", "[beyond.core.container.at_opt]")
{
  SECTION("Raw array")
  {
    constexpr int ca[] = {1, 2, 3, 4, 5};
    const int a[] = {1, 2, 3, 4, 5};

    STATIC_REQUIRE(beyond::at_opt(ca, 0) == 1);
    REQUIRE(beyond::at_opt(a, 0) == 1);

    STATIC_REQUIRE(beyond::at_opt(ca, 5) == beyond::nullopt);
    REQUIRE(beyond::at_opt(a, 5) == beyond::nullopt);
  }

  SECTION("Mutate raw array")
  {
    int a[] = {1, 2, 3, 4, 5};
    *beyond::at_opt(a, 0) = 42;
    REQUIRE(beyond::at_opt(a, 0) == 42);
  }

  SECTION("std::array")
  {
    constexpr std::array ca = {1, 2, 3, 4, 5};
    const std::array a = ca;

    STATIC_REQUIRE(beyond::at_opt(ca, 0) == 1);
    REQUIRE(beyond::at_opt(a, 0) == 1);

    STATIC_REQUIRE(beyond::at_opt(ca, 5) == beyond::nullopt);
    REQUIRE(beyond::at_opt(a, 5) == beyond::nullopt);
  }

  SECTION("Mutate std::array")
  {
    constexpr auto make_array = []() {
      std::array a = {1, 2, 3, 4, 5};
      *beyond::at_opt(a, 0) = 100;
      return a;
    };
    constexpr std::array ca = make_array();
    STATIC_REQUIRE(beyond::at_opt(ca, 0) == 100);

    std::array a = make_array();
    REQUIRE(beyond::at_opt(a, 0) == 100);
  }

  SECTION("std::vector")
  {
    const std::vector v{1, 2, 3};

    REQUIRE(beyond::at_opt(v, 0) == 1);
    REQUIRE(beyond::at_opt(v, 5) == beyond::nullopt);
  }

  SECTION("Mutate std::vector")
  {
    std::vector v{1, 2, 3};
    *beyond::at_opt(v, 0) = 100;
    REQUIRE(beyond::at_opt(v, 0) == 100);
  }

  SECTION("static vector")
  {
    const beyond::StaticVector<int, 5> a = {1, 2, 3, 4, 5};

    REQUIRE(beyond::at_opt(a, 0) == 1);
    REQUIRE(beyond::at_opt(a, 5) == beyond::nullopt);
  }

  SECTION("Mutate static vector")
  {
    beyond::StaticVector<int, 5> a = {1, 2, 3, 4, 5};
    *beyond::at_opt(a, 0) = 100;
    REQUIRE(beyond::at_opt(a, 0) == 100);
  }
}