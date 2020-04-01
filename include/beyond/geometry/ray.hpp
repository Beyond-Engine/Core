#ifndef BEYOND_CORE_GEOMETRY_RAY_HPP
#define BEYOND_CORE_GEOMETRY_RAY_HPP

#include "../math/point.hpp"
#include "../math/vector.hpp"

namespace beyond {

/**
 * @brief Represent a function of ray F(t)=A+Bt where x and y are two vectors
 * that represent origin and direction
 *
 * @warning Ray does not guarantee that its direction is a unit vector
 */
struct Ray {
  Point3 origin = {0, 0, 0};
  Vec3 direction = {1, 0, 0};

  /**
   * @brief Default constructs a ray with origin at (0,0,0) and facing (1, 0, 0)
   * direciton
   */
  constexpr Ray() = default;

  /**
   * @brief Construct a ray by its origin and direction
   * @related Ray
   */
  constexpr Ray(Point3 a, Vec3 b) : origin{a}, direction{b} {}

  /**
   * @brief Gets the result point after we put the parameter t into the ray
   * function
   */
  [[nodiscard]] constexpr auto operator()(float t) const noexcept -> Point3
  {
    return origin + t * direction;
  }
};

} // namespace beyond

#endif // BEYOND_CORE_GEOMETRY_RAY_HPP
