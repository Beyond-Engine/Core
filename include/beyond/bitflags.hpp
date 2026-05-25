#pragma once

#include <utility>

#include "prelude.hpp"

/**
 * @brief Make a scoped enum bitflag
 *
 * A bit flag supports bitwise or `|`, and `&`, and complement `~` operators
 */
// NOLINTBEGIN
#define BEYOND_MAKE_BITFLAG(Typ)                                               \
  BEYOND_FORCE_INLINE constexpr auto operator|(Typ lhs, Typ rhs)->Typ          \
  {                                                                            \
    return static_cast<Typ>(std::to_underlying(lhs) |                          \
                            std::to_underlying(rhs));                          \
  }                                                                            \
                                                                               \
  BEYOND_FORCE_INLINE constexpr auto operator&(Typ lhs, Typ rhs)->Typ          \
  {                                                                            \
    return static_cast<Typ>(std::to_underlying(lhs) &                          \
                            std::to_underlying(rhs));                          \
  }                                                                            \
                                                                               \
  BEYOND_FORCE_INLINE constexpr auto operator~(BitFlag lhs)->BitFlag           \
  { return static_cast<BitFlag>(~std::to_underlying(lhs)); }
// NOLINTEND
