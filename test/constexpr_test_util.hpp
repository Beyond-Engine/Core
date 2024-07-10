#ifndef BEYOND_CORE_CONSTEXPR_TEST_UTIL_HPP
#define BEYOND_CORE_CONSTEXPR_TEST_UTIL_HPP

#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

#define CONSTEXPR_REQUIRE(expr)                                                \
  if (std::is_constant_evaluated()) {                                          \
    if (not(expr)) { throw std::runtime_error("assertion failed!"); }          \
  } else {                                                                     \
    REQUIRE(expr);                                                             \
  }

#define COMP_TIME_AND_RUNTIME_TEST(expr)                                       \
  STATIC_REQUIRE(expr);                                                        \
  REQUIRE(expr);

// A constexpr "within" function
constexpr auto within(float a, float b, float tolerance = 1e-4) -> bool
{
  float d = a - b;
  if (d == 0) { return true; }
  d = std::abs(d);
  return d / (std::abs(a) + std::abs(b)) < tolerance;
}

#endif // BEYOND_CORE_CONSTEXPR_TEST_UTIL_HPP
