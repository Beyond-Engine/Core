#pragma once
#ifndef BEYOND_CORE_MATRIX_TEST_UTIL_HPP
#define BEYOND_CORE_MATRIX_TEST_UTIL_HPP

#include <catch2/catch.hpp>
#include <fmt/format.h>

template <typename Mat>
auto matrix_approx_match(const Mat& lhs, const Mat& rhs,
                         std::string_view msg = "")
{
  for (std::size_t i = 0; i < lhs.dimension(); ++i) {
    for (std::size_t j = 0; j < lhs.dimension(); ++j) {
      INFO(fmt::format("{}: {} != {}, at ({}, {})", msg, lhs, rhs, i, j));
      REQUIRE(lhs(i, j) == Approx(rhs(i, j)).epsilon(0.01));
    }
  }
}

#endif // BEYOND_CORE_MATRIX_TEST_UTIL_HPP
