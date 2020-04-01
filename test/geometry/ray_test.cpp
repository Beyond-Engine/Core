#include <catch2/catch.hpp>

#include "beyond/geometry/ray.hpp"

TEST_CASE("Ray", "[geometry]")
{
  const beyond::Point3 origin = {1, 1, 1};
  const beyond::Vec3 direction = {0, 0, 1};

  SECTION("Default construct a ray at origin")
  {
    beyond::Ray default_ray;
    REQUIRE(default_ray.origin == beyond::Point3{0, 0, 0});
  }

  SECTION("Construct a ray with input")
  {
    beyond::Ray ray{origin, direction};

    REQUIRE(ray.origin == origin);
    REQUIRE(ray.direction == direction);
  }

  SECTION("Gets correct point at parameter t from the ray function")
  {
    beyond::Ray ray{origin, direction};
    const auto expect_dest = origin + 3.f * direction;
    const auto dest = ray(3);

    REQUIRE(expect_dest.x == Approx(dest.x));
    REQUIRE(expect_dest.y == Approx(dest.y));
    REQUIRE(expect_dest.z == Approx(dest.z));
  }
}
