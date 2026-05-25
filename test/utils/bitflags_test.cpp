#include <catch2/catch_test_macros.hpp>

#include <beyond/bitflags.hpp>

enum class BitFlag { a = 0b1, b = 0b10 };
BEYOND_MAKE_BITFLAG(BitFlag);

TEST_CASE("Bitflag")
{
  using enum BitFlag;
  constexpr BitFlag ab = a | b;
  static_assert(std::to_underlying(ab) == 0b11);

  static_assert((ab & a) == a);

  static_assert(std::to_underlying(~a) == ~1);
}
