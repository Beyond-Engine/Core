#ifndef BEYOND_CORE_TYPES_IN_PLACE_HPP
#define BEYOND_CORE_TYPES_IN_PLACE_HPP

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup types
 * @{
 */

/// @brief A tag type to tell types to construct its value in-place
struct in_place_t {
  explicit in_place_t() = default;
};

/// @brief A tag to tell types to construct its value in-place
static constexpr in_place_t in_place{};

/** @}
 * @} */

} // namespace beyond

#endif // BEYOND_CORE_TYPES_IN_PLACE_HPP
