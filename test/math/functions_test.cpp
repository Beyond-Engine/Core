#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "beyond/math/math.hpp"
#include <cmath>

using Catch::Approx;

TEST_CASE("Scalar Trig functions", "[beyond.core.math]")
{
  using namespace beyond::literals;
  const beyond::TDegree<float> d = 60._deg;
  const beyond::TRadian<float> r = d;

  SECTION("sin")
  {
    const auto expected = std::sin(r.value());
    REQUIRE(beyond::sin(r) == Approx(expected));
    REQUIRE(beyond::sin(d) == Approx(expected));
  }

  SECTION("cos")
  {
    const auto expected = std::cos(r.value());
    REQUIRE(beyond::cos(r) == Approx(expected));
    REQUIRE(beyond::cos(d) == Approx(expected));
  }

  SECTION("tan")
  {
    const auto expected = std::tan(r.value());
    REQUIRE(beyond::tan(r) == Approx(expected));
    REQUIRE(beyond::tan(d) == Approx(expected));
  }

  SECTION("asin")
  {
    // Floating points overload
    const auto expected = std::asin(0.5);
    REQUIRE(beyond::asin(0.5).value() == Approx(expected));

    // Integer overload
    const auto expected2 = std::asin(1);
    REQUIRE(beyond::asin(1).value() == Approx(expected2));
  }

  SECTION("acos")
  {
    // Floating points overload
    const auto expected = std::acos(0.5);
    REQUIRE(beyond::acos(0.5).value() == Approx(expected));

    // Integer overload
    const auto expected2 = std::acos(1);
    REQUIRE(beyond::acos(1).value() == Approx(expected2));
  }

  SECTION("atan")
  {
    // Floating points overload
    const auto expected = std::atan(0.5);
    REQUIRE(beyond::atan(0.5).value() == Approx(expected));

    // Integer overload
    const auto expected2 = std::atan(1);
    REQUIRE(beyond::atan(1).value() == Approx(expected2));
  }

  SECTION("atan2")
  {
    // Floating points overload
    const auto expected = std::atan2(2., 1);
    REQUIRE(beyond::atan2(2., 1).value() == Approx(expected));
    // Integer overload
    REQUIRE(beyond::atan2(2, 1).value() == Approx(expected));
  }
}

TEST_CASE("Scalar lerp", "[beyond.core.math]")
{
  REQUIRE(beyond::lerp(0, 1, 0.0f) == 0);
  REQUIRE(beyond::lerp(0, 1, 1.0f) == 1);
  REQUIRE(beyond::lerp(0, 1, 0.3f) == Approx(0.3f));
}
