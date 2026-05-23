#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import beyond.core;

#include "../constexpr_test_util.hpp"

#include <fmt/format.h>

using beyond::BiVec3;
using beyond::Rotor3;
using beyond::Vec3;

static_assert(sizeof(Rotor3) == 4 * sizeof(float));

template <Rotor3 actual, Rotor3 expected> constexpr auto rotor3_check()
{
  if (std::is_constant_evaluated()) {
    CONSTEXPR_REQUIRE(within(actual.a_, expected.a_));
    CONSTEXPR_REQUIRE(within(actual.xy_, expected.xy_));
    CONSTEXPR_REQUIRE(within(actual.xz_, expected.xz_));
    CONSTEXPR_REQUIRE(within(actual.yz_, expected.yz_));
  } else {
    CHECK(actual.a_ == Catch::Approx(expected.a_));
    CHECK(actual.xy_ == Catch::Approx(expected.xy_));
    CHECK(actual.xz_ == Catch::Approx(expected.xz_));
    REQUIRE(actual.yz_ == Catch::Approx(expected.yz_));
  }
  return true;
}

TEST_CASE("Rotor construction", "[beyond.core.math.rotor]")
{
  static constexpr auto one_rotor = Rotor3(1.0, 0.0, 0.0, 0.0);

  SECTION("Default rotor")
  {
    static constexpr Rotor3 default_rotor;
    COMP_TIME_AND_RUNTIME_TEST((rotor3_check<default_rotor, one_rotor>()));
  }

  SECTION("Rotor from a scalar and bivector")
  {
    static constexpr auto actual = Rotor3(1.0, BiVec3(2.0, 3.0, 4.0));
    static constexpr auto expected = Rotor3(1.0, 2.0, 3.0, 4.0);
    COMP_TIME_AND_RUNTIME_TEST((rotor3_check<actual, expected>()));
  }

  SECTION("Rotor from two vectors (geometric product)")
  {
    static constexpr auto e1 = Vec3(1, 0, 0);
    static constexpr auto e2 = Vec3(0, 1, 0);
    static constexpr auto e3 = Vec3(0, 0, 1);

    COMP_TIME_AND_RUNTIME_TEST(Rotor3(e1, e1) == one_rotor);
    COMP_TIME_AND_RUNTIME_TEST(Rotor3(e2, e2) == one_rotor);
    COMP_TIME_AND_RUNTIME_TEST(Rotor3(e3, e3) == one_rotor);
  }
}