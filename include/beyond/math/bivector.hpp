#ifndef BEYOND_CORE_MATH_BIVECTOR_HPP
#define BEYOND_CORE_MATH_BIVECTOR_HPP

#include "./matrix.hpp"
#include "./vector.hpp"

namespace beyond {

/**
 * @brief A bivector (a.k.a oriented-area) in 3D
 *
 * In 3D, a bivector has 3 components, each represented a each one representing
 * the *signed projected area* of the bivector onto one of the 3 *basis
 * bivectors*.
 */
struct BiVec3 {
  float xy = 0;
  float xz = 0;
  float yz = 0;

  constexpr BiVec3() = default;
  constexpr BiVec3(float xy, float xz, float yz) : xy{xy}, xz{xz}, yz{yz} {}

  /// @return The zero bivector
  [[nodiscard]]
  static constexpr auto zero() -> BiVec3
  {
    return {0, 0, 0};
  }

  /// @return The unit bivector that represents the xy_ plane
  [[nodiscard]]
  static constexpr auto xy_unit() -> BiVec3
  {
    return {1, 0, 0};
  }

  /// @return The unit bivector that represents the xz_ plane
  [[nodiscard]]
  static constexpr auto xz_unit() -> BiVec3
  {
    return {0, 1, 0};
  }

  /// @return The unit bivector that represents the yz_ plane
  [[nodiscard]]
  static constexpr auto yz_unit() -> BiVec3
  {
    return {0, 0, 1};
  }
};

// @brief wedge product
[[nodiscard]] constexpr auto wedge(Vec3 u, Vec3 v) -> BiVec3
{
  return {
      u[0] * v[1] - u[1] * v[0], // XY
      u[0] * v[2] - u[2] * v[0], // XZ
      u[1] * v[2] - u[2] * v[1]  // YZ
  };
}

} // namespace beyond

#endif // BEYOND_CORE_MATH_BIVECTOR_HPP
