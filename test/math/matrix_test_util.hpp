#pragma once
#ifndef BEYOND_CORE_MATRIX_TEST_UTIL_HPP
#define BEYOND_CORE_MATRIX_TEST_UTIL_HPP

#include <catch2/catch.hpp>
#include <fmt/format.h>

template <typename Vec>
auto vector_approx_match(const Vec& lhs, const Vec& rhs,
                         std::string_view msg = "")
{
  for (std::size_t i = 0; i < lhs.dimension(); ++i) {
    INFO(fmt::format("{}: {} != {}, at {}", msg, lhs, rhs, i));
    REQUIRE(lhs(i) == Approx(rhs(i)).epsilon(0.01).margin(1e-5));
  }
}

template <typename Mat>
auto matrix_approx_match(const Mat& lhs, const Mat& rhs,
                         std::string_view msg = "")
{
  for (std::size_t i = 0; i < lhs.dimension(); ++i) {
    for (std::size_t j = 0; j < lhs.dimension(); ++j) {
      INFO(fmt::format("{}: {} != {}, at ({}, {})", msg, lhs, rhs, i, j));
      REQUIRE(lhs(i, j) == Approx(rhs(i, j)).epsilon(0.01).margin(1e-5));
    }
  }
}

#endif // BEYOND_CORE_MATRIX_TEST_UTIL_HPP
