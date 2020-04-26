#include <catch2/catch.hpp>

#include <array>
#include <beyond/container/static_vector.hpp>
#include <string>

#include "../raii_counter.hpp"

using namespace beyond;

TEST_CASE("StaticVector", "[beyond.core.container.static_vector]")
{
  GIVEN("A default constructed StaticVector")
  {
    StaticVector<int, 10> v1;
    REQUIRE(v1.capacity() == 10);
    THEN("it is empty")
    {
      REQUIRE(v1.empty());
      REQUIRE(v1.size() == 0);
    }

    WHEN("Adds an element to it")
    {
      const int first = 42;
      v1.emplace_back(first);

      THEN("Can find that element at front")
      {
        REQUIRE(v1[0] == first);
      }

      THEN("It is no longer empty")
      {
        REQUIRE(!v1.empty());
        REQUIRE(v1.size() == 1);
      }

      AND_WHEN("Adds another element to it")
      {
        const int second = 21;
        v1.push_back(second);
        REQUIRE(v1.size() == 2);
        REQUIRE(v1[1] == second);

        AND_WHEN("pops the last element from it")
        {
          v1.pop_back();
          REQUIRE(v1.size() == 1);
          REQUIRE(v1[0] == first);
        }

        AND_THEN("Clear the vector")
        {
          v1.clear();
          REQUIRE(v1.size() == 0);
        }
      }
    }
  }
}

TEST_CASE("StaticVector constructors", "[beyond.core.container.static_vector]")
{
  SECTION("construct by size")
  {
    StaticVector<int, 10> v(8);
    REQUIRE(v.size() == 8);
    REQUIRE(v[1] == int{});
  }

  SECTION("construct by size and a value")
  {
    StaticVector<int, 10> v(8, 42);
    REQUIRE(v.size() == 8);
    REQUIRE(v[7] == 42);
  }

  SECTION("construct by a pair of iterator")
  {
    std::array a{1, 2, 3, 4, 5};
    StaticVector<int, 10> v{a.begin(), a.end()};
    REQUIRE(v.size() == 5);
    REQUIRE(v[3] == 4);
  }

  SECTION("construct by an initializer list")
  {
    StaticVector<int, 10> v{1, 2, 3, 4, 5};
    REQUIRE(v.size() == 5);
    REQUIRE(v[3] == 4);
  }
}

TEST_CASE("StaticVector copies and moves",
          "[beyond.core.container.static_vector]")
{
  Counters counters;
  StaticVector<Small, 3> v{Small{counters}};
  CHECK(counters.constructor == 1);
  CHECK(counters.copy == 1);
  CHECK(counters.move == 0);
  CHECK(counters.destructor == 1);

  SECTION("Copy constructor")
  {
    StaticVector<Small, 3> v2{v};
    CHECK(counters.constructor == 1);
    CHECK(counters.copy == 2);
    CHECK(counters.move == 0);
    CHECK(counters.destructor == 1);
  }

  SECTION("Copy assignment")
  {
    Counters counters2;
    StaticVector<Small, 3> v2{counters2};
    v2 = v;
    CHECK(counters.constructor == 1);
    CHECK(counters.copy == 2);
    CHECK(counters.move == 0);
    CHECK(counters.destructor == 1);

    CHECK(counters2.constructor == 1);
    CHECK(counters2.copy == 1);
    CHECK(counters2.move == 0);
    CHECK(counters2.destructor == 2);
  }

  SECTION("Copy assignment self")
  {
    v = v;
    CHECK(counters.constructor == 1);
    CHECK(counters.copy == 1);
    CHECK(counters.move == 0);
    CHECK(counters.destructor == 1);
  }

  SECTION("Move constructor")
  {
    StaticVector<Small, 3> v2{std::move(v)};
    CHECK(counters.constructor == 1);
    CHECK(counters.copy == 1);
    CHECK(counters.move == 1);
    CHECK(counters.destructor == 1);
  }
}

TEST_CASE("StaticVector accessors", "[beyond.core.container.static_vector]")
{
  StaticVector<int, 10> v{1, 2, 3, 4, 5};
  const StaticVector<int, 10> cv{v};

  REQUIRE(v[1] == 2);
  REQUIRE(cv[4] == 5);

  REQUIRE(v.front() == 1);
  v.front() = 2;
  REQUIRE(v.front() == 2);
  REQUIRE(cv.front() == 1);

  REQUIRE(v.back() == 5);
  v.back() = 2;
  REQUIRE(v.back() == 2);
  REQUIRE(cv.back() == 5);

  REQUIRE(v.data() == &v.front());
  REQUIRE(cv.data() == &cv.front());

  REQUIRE(v.at(2) == 3);
  REQUIRE_THROWS_AS(v.at(6), std::out_of_range);
  REQUIRE(cv.at(3) == 4);
  REQUIRE_THROWS_AS(cv.at(6), std::out_of_range);
}

TEST_CASE("StaticVector swap", "[beyond.core.container.static_vector]")
{
  const auto l1 = {1, 2, 3, 4, 5};
  const auto l2 = {2, 4, 5};

  StaticVector<int, 10> v1{l1.begin(), l1.end()};
  StaticVector<int, 10> v2{l2.begin(), l2.end()};

  SECTION(".swap() member")
  {
    v1.swap(v2);
  }

  SECTION("swap free function")
  {
    swap(v1, v2);
  }

  REQUIRE(std::equal(v1.cbegin(), v1.cend(), l2.begin()));
  REQUIRE(std::equal(v2.cbegin(), v2.cend(), l1.begin()));
}

TEST_CASE("StaticVector iterators", "[beyond.core.container.static_vector]")
{
  GIVEN("An empty StaticVector")
  {
    StaticVector<std::string, 10> v;
    REQUIRE(v.begin() == v.end());
    REQUIRE(v.cbegin() == v.cend());

    const std::string first{"hello"};
    v.push_back(first);
    REQUIRE(v.begin() != v.end());
    REQUIRE(v.cbegin() != v.cend());

    SECTION("Elements access")
    {
      REQUIRE(*v.begin() == first);
      REQUIRE(v.begin()->size() == first.size());
    }

    SECTION("Pre and post fix increment & decrement")
    {
      const std::string second{"world"};
      v.push_back(second);
      auto i = v.begin();
      REQUIRE(*(++i) == second);
      REQUIRE(*i == second);
      REQUIRE(*(i++) == second);
      REQUIRE(i == v.end());
      REQUIRE((i--) == v.end());
      REQUIRE(*i == second);
      REQUIRE(*(--i) == first);
      REQUIRE(*i == first);
    }

    SECTION("Iterator ordering")
    {
      REQUIRE(v.begin() < v.end());
      REQUIRE(v.begin() <= v.end());
      REQUIRE(!(v.begin() > v.end()));
      REQUIRE(!(v.begin() >= v.end()));
    }
  }

  GIVEN("A StaticVector {1, 2, 3, 4}")
  {
    StaticVector<int, 8> v{1, 2, 3};

    AND_GIVEN("i1 = v.begin()")
    {
      auto i1 = v.begin();
      AND_WHEN("i1 += 3")
      {
        i1 += 3;
        AND_THEN("i1 is at v.end()")
        {
          REQUIRE(i1 == v.end());
        }

        AND_WHEN("i1 -= 2")
        {
          i1 -= 2;
          AND_THEN("*i1 == 2")
          {
            REQUIRE(*i1 == 2);
          }
        }
      }
    }

    SECTION("Random access iterator affine space operations")
    {
      const auto ssize = static_cast<decltype(v)::difference_type>(v.size());
      REQUIRE(v.end() - v.begin() == ssize);
      REQUIRE(v.begin() + ssize == v.end());
      REQUIRE(ssize + v.begin() == v.end());
    }
  }
}
