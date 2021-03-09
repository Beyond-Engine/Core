#include <catch2/catch.hpp>
#include <sstream>
#include <string>

#include <beyond/math/quat.hpp>
#include <beyond/math/serial.hpp>

#include "../serial_test_util.hpp"
#include "matrix_test_util.hpp"

using beyond::DQuat;
using beyond::Quat;
using beyond::TQuat;
using beyond::Vec3;

template <typename T>
void quat_match(const beyond::TQuat<T>& result,
                const beyond::TQuat<T>& expected)
{
  CHECK(result.w == Approx(expected.w));
  CHECK(result.x == Approx(expected.x));
  CHECK(result.y == Approx(expected.y));
  CHECK(result.z == Approx(expected.z));
}

TEST_CASE("Quaternion default constructor", "[beyond.core.math.quaternion]")
{
  constexpr Quat cq;
  STATIC_REQUIRE(cq.x == 0);
  STATIC_REQUIRE(cq.y == 0);
  STATIC_REQUIRE(cq.z == 0);
  STATIC_REQUIRE(cq.w == 0);

  const Quat q;
  REQUIRE(q.x == 0);
  REQUIRE(q.y == 0);
  REQUIRE(q.z == 0);
  REQUIRE(q.w == 0);
}

TEST_CASE("Quaternion real-img constructor", "[beyond.core.math.quaternion]")
{
  constexpr Quat cq{1, {2, 3, 4}};
  STATIC_REQUIRE(cq.w == 1);
  STATIC_REQUIRE(cq.x == 2);
  STATIC_REQUIRE(cq.y == 3);
  STATIC_REQUIRE(cq.z == 4);

  const Quat q{1, {2, 3, 4}};
  REQUIRE(q.w == 1);
  REQUIRE(q.x == 2);
  REQUIRE(q.y == 3);
  REQUIRE(q.z == 4);
}

TEST_CASE("Quaternion component-wise constructor",
          "[beyond.core.math.quaternion]")
{
  constexpr Quat cq{1, 2, 3, 4};
  STATIC_REQUIRE(cq.w == 1);
  STATIC_REQUIRE(cq.x == 2);
  STATIC_REQUIRE(cq.y == 3);
  STATIC_REQUIRE(cq.z == 4);

  const Quat q{1, 2, 3, 4};
  REQUIRE(q.w == 1);
  REQUIRE(q.x == 2);
  REQUIRE(q.y == 3);
  REQUIRE(q.z == 4);
}

TEST_CASE("Quaternion equality", "[beyond.core.math.quaternion]")
{
  constexpr Quat cq1{1, {2, 3, 4}};
  constexpr Quat cq1_{1, {2, 3, 4}};
  constexpr Quat cq2{5, {6, 7, 8}};
  STATIC_REQUIRE(cq1 == cq1_);
  STATIC_REQUIRE(cq1 != cq2);

  const Quat q1{1, {2, 3, 4}};
  const Quat q1_{1, {2, 3, 4}};
  const Quat q2{5, {6, 7, 8}};
  REQUIRE(q1 == q1_);
  REQUIRE(q1 != q2);
}

TEST_CASE("Quaternion product", "[beyond.core.math.quaternion]")
{
  constexpr Quat cq1{1, {2, 3, 4}};
  constexpr Quat cq2{5, {6, 7, 8}};

  constexpr Quat expected{-8, {30, 24, -60}};

  constexpr Quat c_result = cq1 * cq2;
  quat_match(c_result, expected);

  const Quat result = cq1 * cq2;
  quat_match(result, expected);
}

TEST_CASE("Quaternion serialization", "[beyond.core.math.quaternion]")
{
  constexpr Quat q{1, {2, 3, 4}};

  REQUIRE_TO_STRING_EQ(q, "quat(1, (2, 3, 4))");
}