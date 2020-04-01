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
  /// @param value seed value to use in the initialization of the internal state
  explicit constexpr xorshift32(result_type value) noexcept : state{value} {}

  /// @brief Reinitializes the internal state of the random-number engine using
  /// new seed value.
  /// @warning The seed must be initialized to none-zero
  /// @param value seed value to use in the initialization of the internal state
  auto seed(result_type value = default_seed) -> void
  {
    state = value;
  }

  /// @brief advances the engine's state and returns the generated value
  [[nodiscard]] constexpr auto operator()() noexcept -> result_type
  {
    result_type x = state;
    x ^= x << 13u;
    x ^= x >> 17u;
    x ^= x << 15u;
    state = x;
    return x;
  }

  /// @brief  advances the engine's state by a specified amount
  constexpr auto discard(unsigned long long z) noexcept -> void
  {
    result_type x = state;
    for (unsigned long long i = 0; i < z; ++i) {
      x ^= x << 13u;
      x ^= x >> 17u;
      x ^= x << 15u;
    }
    state = x;
  }

  /// @brief gets the smallest possible value in the output range
  [[nodiscard]] static constexpr auto min() noexcept -> result_type
  {
    return std::numeric_limits<std::uint32_t>::min();
  }

  /// @brief gets the largest possible value in the output range
  [[nodiscard]] static constexpr auto max() noexcept -> result_type
  {
    return std::numeric_limits<std::uint32_t>::max();
  }

  /// @brief Compares two pseudo-random number engines.
  ///
  /// Two engines are equal, if their internal states are equivalent, that is,
  /// if they would generate equivalent values for any number of calls of
  /// operator().
  [[nodiscard]] friend constexpr auto operator==(xorshift32 lhs,
                                                 xorshift32 rhs) noexcept
      -> bool
  {
    return lhs.state == rhs.state;
  }

  [[nodiscard]] friend constexpr auto operator!=(xorshift32 lhs,
                                                 xorshift32 rhs) noexcept
      -> bool
  {
    return lhs.state != rhs.state;
  }
};

/** @}
 *  @} */

} // namespace beyond

#endif // BEYOND_CORE_RANDOM_XORSHIFT32_HPP
