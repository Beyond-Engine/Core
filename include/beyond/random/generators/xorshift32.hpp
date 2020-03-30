#ifndef BEYOND_CORE_RANDOM_XORSHIFT32_HPP
#define BEYOND_CORE_RANDOM_XORSHIFT32_HPP

#include <cstdint>
#include <limits>
#include <type_traits>

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup random
 * @{
 */

/**
 * @brief xorshift32 random number generator
 */
struct xorshift32 {
  using result_type = std::uint32_t;
  static constexpr result_type default_seed = 1u;

  result_type state = default_seed;

  constexpr xorshift32() noexcept = default;

  /// @brief Initializes an xorshift32 generator with seed
  /// @warning The seed must be initialized to none-zero
  explicit constexpr xorshift32(result_type seed) noexcept : state{seed} {}

  constexpr auto operator()() noexcept -> result_type
  {
    result_type x = state;
    x ^= x << 13u;
    x ^= x >> 17u;
    x ^= x << 15u;
    state = x;
    return x;
  }

  static constexpr auto min() noexcept -> result_type
  {
    return std::numeric_limits<std::uint32_t>::min();
  }

  static constexpr auto max() noexcept -> result_type
  {
    return std::numeric_limits<std::uint32_t>::max();
  }
};

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_RANDOM_XORSHIFT32_HPP
