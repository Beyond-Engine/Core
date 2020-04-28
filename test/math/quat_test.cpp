#include <beyond/math/vector.hpp>

namespace beyond {

/**
 * @brief A quaternion type
 */
template <typename T> struct TQuat {
  T x, y, z, w;

  /// @brief Default constructor
  constexpr TQuat() noexcept : x{0}, y{0}, z{0}, w{0} {}

  /// @brief Component-wise constructor
  explicit constexpr TQuat(T w_, T x_, T y_, T z_) noexcept
      : x{x_}, y{y_}, z{z_}, w{w_}
  {
  }

  /// @brief Creates quaternion from a scalar and a vector
  explicit constexpr TQuat(T real, const TVec3<T>& v) noexcept
      : x{v.elem[0]}, y{v.elem[1]}, z{v.elem[2]}, w{real}
  {
  }

  [[nodiscard]] friend constexpr auto operator*(const TQuat<T>& lhs,
                                                const TQuat<T>& rhs) -> TQuat<T>
  {
    return TQuat<T>(
        lhs.w * rhs.x - lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
  }
};

using Quat = TQuat<float>;
using DQuat = TQuat<double>;

} // namespace beyond

#include <catch2/catch.hpp>

using beyond::DQuat;
using beyond::Quat;
using beyond::TQuat;

template <typename T>
void quad_match(const beyond::TQuat<T>& result,
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

TEST_CASE("Quaternion product", "[beyond.core.math.quaternion]")
{
  constexpr Quat cq1{1, {2, 3, 4}};
  constexpr Quat cq2{5, {6, 7, 8}};

  constexpr Quat expected{-8, {30, 24, -60}};

  constexpr Quat c_result = cq1 * cq2;
  quad_match(c_result, expected);

  const Quat result = cq1 * cq2;
  quad_match(result, expected);
}