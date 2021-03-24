#include <catch2/catch.hpp>

#include <beyond/container/array.hpp>

#include <algorithm>

TEST_CASE("Array default initialization", "[beyond.core.container.array]")
{
  beyond::Array<int, 5> a;
}

TEST_CASE("Array value initialization", "[beyond.core.container.array]")
{
  constexpr beyond::Array<int, 5> ca{};
  STATIC_REQUIRE_FALSE(ca.empty());
  STATIC_REQUIRE(ca.size() == 5);
  STATIC_REQUIRE(ca.max_size() == 5);
  STATIC_REQUIRE(
      std::all_of(ca.begin(), ca.end(), [](int i) { return i == 0; }));
}

TEST_CASE("Array member functions", "[beyond.core.container.array]")
{

  GIVEN("An aggregated initialized array")
  {
    constexpr auto create_array = []() {
      return beyond::Array<int, 5>{1, 2, 3};
    };
    constexpr beyond::Array<int, 5> ca = create_array();

    THEN("Its values are as expected")
    {
      STATIC_REQUIRE(ca[0] == 1);
      STATIC_REQUIRE(ca[1] == 2);
      STATIC_REQUIRE(ca[2] == 3);
      STATIC_REQUIRE(
          std::all_of(ca.begin() + 3, ca.end(), [](int i) { return i == 0; }));
    }

    THEN("We can mutate the array by indices")
    {
      constexpr auto ca2 = [&]() {
        auto a = create_array();
        a[4] = 42;
        return a;
      }();
      STATIC_REQUIRE(ca2[4] == 42);
    }

    THEN("We can get the front of that array")
    {
      STATIC_REQUIRE(ca.front() == 1);
    }

    THEN("We can mutate the front of that array")
    {
      constexpr auto ca2 = [&]() {
        auto a = create_array();
        a.front() = 42;
        return a;
      }();
      STATIC_REQUIRE(ca2[0] == 42);
    }

    THEN("We can get the back of that array")
    {
      STATIC_REQUIRE(ca.back() == 0);
    }

    THEN("We can mutate the back of that array")
    {
      constexpr auto ca2 = [&]() {
        auto a = create_array();
        a.back() = 42;
        return a;
      }();
      STATIC_REQUIRE(ca2[4] == 42);
    }

    THEN("We can access a pointer to the underlying raw array")
    {
      STATIC_REQUIRE(ca.data()[1] == 2);
    }

    THEN("We can mutate by .data()")
    {
      constexpr auto ca2 = [&]() {
        auto a = create_array();
        a.data()[1] = 42;
        return a;
      }();
      STATIC_REQUIRE(ca2[1] == 42);
    }

    // TODO: remove this once no longer support GCC10
#if !defined(__GNUC__) || __GNUC__ > 10
    THEN("We can check the equality of two arrays")
    {
      constexpr auto ca2 = ca;
      STATIC_REQUIRE(ca == ca2);
    }
#endif

    THEN("We can fill the array")
    {
      constexpr auto ca2 = [&]() {
        auto a = create_array();
        a.fill(42);
        return a;
      }();
      STATIC_REQUIRE(
          std::all_of(ca2.begin(), ca2.end(), [](int i) { return i == 42; }));
    }

    THEN("We can swap two arrays")
    {
      constexpr auto p = [&]() {
        auto a = create_array();
        auto b = a;
        a.fill(42);
        swap(a, b);
        return std::pair(a, b);
      }();
      constexpr auto a = p.first;
      constexpr auto b = p.second;
      STATIC_REQUIRE(a[0] == 1);
      STATIC_REQUIRE(a[1] == 2);
      STATIC_REQUIRE(a[2] == 3);
      STATIC_REQUIRE(
          std::all_of(a.begin() + 3, a.end(), [](int i) { return i == 0; }));
      STATIC_REQUIRE(
          std::all_of(b.begin(), b.end(), [](int i) { return i == 42; }));
    }

    THEN("We can read the reverse of the array")
    {
      constexpr auto cra = [&]() {
        auto ra = create_array();
        std::reverse(ra.begin(), ra.end());
        return ra;
      }();
      STATIC_REQUIRE(std::equal(ca.rbegin(), ca.rend(), cra.begin()));
    }

    // TODO: remove this once no longer support GCC10
#if !defined(__GNUC__) || __GNUC__ > 10
    THEN("We can compare arrays")
    {
      constexpr beyond::Array<int, 5> ca2{1, 2, 3, 4};
      STATIC_REQUIRE(ca2 > ca);
    }
#endif

    THEN("We can do structure binding on it")
    {
      constexpr beyond::Array<int, 3> a{1, 2, 3};
      const auto [a0, a1, a2] = a;
      REQUIRE(a0 == 1);
      REQUIRE(a1 == 2);
      REQUIRE(a2 == 3);
    }
  }
}