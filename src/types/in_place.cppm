export module beyond.core:in_place;

export namespace beyond {

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
inline constexpr in_place_t in_place{};

/** @}
 * @} */

} // namespace beyond
