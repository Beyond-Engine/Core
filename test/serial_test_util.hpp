#pragma once
#ifndef BEYOND_CORE_SERIAL_TEST_UTIL_HPP
#define BEYOND_CORE_SERIAL_TEST_UTIL_HPP

#include <catch2/catch.hpp>
#include <sstream>

#define REQUIRE_TO_STRING_EQ(data, expected)                                   \
  do {                                                                         \
    REQUIRE(to_string(data) == (expected));                                    \
    {                                                                          \
      std::stringstream ss;                                                    \
      ss << (data);                                                            \
      REQUIRE((ss).str() == (expected));                                       \
    }                                                                          \
  } while (false)

#endif // BEYOND_CORE_SERIAL_TEST_UTIL_HPP
