#include <catch2/catch.hpp>
#include <sstream>

#include "beyond/geometry/aabb3.hpp"

static constexpr float inf = std::numeric_limits<float>::infinity();

using beyond::AABB3;
using beyond::Point3;

TEST_CASE("AABB3 construction", "AABB3")
{
  SECTION("Default constructed AABB3 is at (0, 0, 0)")
  {
    const AABB3 box{};
    REQUIRE(box.min() == Point3{0, 0, 0});
    REQUIRE(box.max() == Point3{0, 0, 0});
  }

  SECTION("Create AABB3 with one point")
  {
    const Point3 p{1, 2, 2};
    const AABB3 box{p};
    REQUIRE(box.min() == p);
    REQUIRE(box.max() == p);
  }

  SECTION("Create AABB3 with two points")
  {
    const Point3 p1{1, 2, 2};
    const Point3 p2{-1, 5, 4};

    const AABB3 box{p1, p2};
    REQUIRE(box.min() == Point3{-1, 2, 2});
    REQUIRE(box.max() == Point3{1, 5, 4});
  }

  SECTION("Create AABB3 with min and max points unchecked")
  {
    const Point3 p1{1, 2, 2};
    const Point3 p2{-1, 5, 4};

    const AABB3 box{p1, p2, AABB3::unchecked_tag};
    REQUIRE(box.min() == p1);
    REQUIRE(box.max() == p2);
  }
}

TEST_CASE("Ray/AABB3 intersection", "[AABB3]")
{
  const AABB3 box({0, 0, 0}, {1, 1, 1}, AABB3::unchecked_tag);

  SECTION("Intersect with ray that penetrates its middle")
  {
    const beyond::Ray r({0.5, -1, 0.5}, {0, 1, 0});
    REQUIRE(box.is_intersect_with(r, 0, inf));
  }

  SECTION("Intersect with ray that penetrates its conner")
  {
    const beyond::Ray r({0, -0.5f, 0}, {0, 1, 1});
    REQUIRE(box.is_intersect_with(r, 0, inf));
  }

  SECTION("Not intersect with ray of divert direction")
  {
    const beyond::Ray r({0, -1, 0}, {1, 1, 1});
    REQUIRE(!box.is_intersect_with(r, 0, inf));
  }

  SECTION("Not intersect with ray of negative direction")
  {
    const beyond::Ray r({0.5, -1, 0.5}, {0, -1, 0});
    REQUIRE(!box.is_intersect_with(r, 0, inf));
  }

  SECTION("Not intersect with limited t")
  {
    const beyond::Ray r({0.5f, -1, 0.5f}, {0, 1, 0});
    REQUIRE(!box.is_intersect_with(r, 0, 0.9f));
  }
}

TEST_CASE("AABB3 union", "[AABB3]")
{
  AABB3 box0{{0, 0, 0}, {1, 1, 1}, AABB3::unchecked_tag};
  AABB3 box1{{-1, -1, -1}, {0.5, 0.5, 0.5}, AABB3::unchecked_tag};
  REQUIRE(merge(box0, box1) ==
          AABB3{{-1, -1, -1}, {1, 1, 1}, AABB3::unchecked_tag});
}

TEST_CASE("AABB3 Serialization", "[AABB3]")
{
  const auto expected =
      "AABB3(min: point(0.0, 0.0, 0.0), max: point(1.0, 1.0, 1.0))";

  AABB3 box{{0, 0, 0}, {1, 1, 1}, AABB3::unchecked_tag};
  REQUIRE(to_string(box) == expected);

  std::stringstream ss;
  ss << box;
  REQUIRE(ss.str() == expected);
}
