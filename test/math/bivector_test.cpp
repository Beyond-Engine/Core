#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../constexpr_test_util.hpp"
#include "beyond/math/bivector.hpp"

using beyond::BiVec3;
using beyond::Vec3;

template <BiVec3 actual, BiVec3 expected>
constexpr void comptime_require_bivector_approx_eq()
{
  CONSTEXPR_REQUIRE(within(actual.xy, expected.xy));
  CONSTEXPR_REQUIRE(within(actual.xz, expected.xz));
  CONSTEXPR_REQUIRE(within(actual.yz, expected.yz));
}

void require_bivector_approx_eq(BiVec3 actual, BiVec3 expected)
{
  CHECK(actual.xy == Catch::Approx(expected.xy));
  CHECK(actual.xz == Catch::Approx(expected.xz));
  REQUIRE(actual.yz == Catch::Approx(expected.yz));
}

template <Vec3 v1, Vec3 v2, BiVec3 expected> constexpr auto wedge_test()
{
  if (std::is_constant_evaluated()) {
    comptime_require_bivector_approx_eq<wedge(v1, v2), expected>();
  } else {
    require_bivector_approx_eq(wedge(v1, v2), expected);
  }
  return true;
}

TEST_CASE("Constants")
{
  require_bivector_approx_eq(BiVec3::zero(), BiVec3(0.0, 0.0, 0.0));
  require_bivector_approx_eq(BiVec3::xy_unit(), BiVec3(1.0, 0.0, 0.0));
  require_bivector_approx_eq(BiVec3::xz_unit(), BiVec3(0.0, 1.0, 0.0));
  require_bivector_approx_eq(BiVec3::yz_unit(), BiVec3(0.0, 0.0, 1.0));
}

TEST_CASE("Wedge Product")
{
  static constexpr Vec3 e1{1.0, 0.0, 0.0};
  static constexpr Vec3 e2{0.0, 1.0, 0.0};
  static constexpr Vec3 e3{0.0, 0.0, 1.0};

  static constexpr auto zero = BiVec3::zero();
  static constexpr BiVec3 xy = BiVec3::xy_unit();
  static constexpr BiVec3 xz = BiVec3::xz_unit();
  static constexpr BiVec3 yz = BiVec3::yz_unit();

  SECTION("wedge(v, v) == 0")
  {
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<e1, e1, zero>()));
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<e2, e2, zero>()));
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<e3, e3, zero>()));

    static constexpr Vec3 v{1.0, 2.0, 3.0};
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<v, v, zero>()));
  }

  SECTION("wedge unit vectors")
  {
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<e1, e2, xy>()));
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<e1, e3, xz>()));
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<e2, e3, yz>()));
  }

  SECTION("adhoc vectors wedge product")
  {
    static constexpr Vec3 v1{1.0, 2.0, 3.0};
    static constexpr Vec3 v2{4.0, 5.0, 6.0};
    static constexpr BiVec3 expected{-3.0, -6.0, -3.0};
    COMP_TIME_AND_RUNTIME_TEST((wedge_test<v1, v2, expected>()));
  }
}