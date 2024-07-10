#ifndef BEYOND_CORE_MATH_ROTOR_HPP
#define BEYOND_CORE_MATH_ROTOR_HPP

#include "beyond/math/bivector.hpp"

namespace beyond {

/**
 * @brief Rotor3 represents a rotation in 3D
 */
struct Rotor3 {
  // scalar part
  float a_ = 1;
  // bivector part
  float xy_ = 0;
  float xz_ = 0;
  float yz_ = 0;

public:
  constexpr Rotor3() = default;
  constexpr Rotor3(float a, float xy, float xz, float yz)
      : a_(a), xy_(xy), xz_(xz), yz_(yz)
  {
  }

  // @brief Constructs the rotor from a scalar and a bivector
  constexpr Rotor3(float a, BiVec3 bv)
      : a_(a), xy_(bv.xy), xz_(bv.xz), yz_(bv.yz)
  {
  }

  // @brief Constructs the rotor that rotates one vector to another
  constexpr Rotor3(Vec3 from, Vec3 to);

  // @brief Constructs the rotor from angle+plane
  //
  // This is analogous to axis-angle rotation in 3x3 matrices or quaternions
  Rotor3(const BiVec3& bv_plane, Radian angle);

  // rotate a vector by the rotor
  [[nodiscard]] constexpr auto rotate(const Vec3& v) const -> Vec3;

  // multiply
  [[nodiscard]] constexpr auto operator*(const Rotor3& r) const -> Rotor3;
  constexpr auto operator*=(const Rotor3& r) -> Rotor3&;
  [[nodiscard]] constexpr auto rotate(Rotor3 r) const -> Rotor3;

  // length utility
  [[nodiscard]]
  constexpr auto reverse() const -> Rotor3; // equivalent to conjugate

  [[nodiscard]]
  constexpr auto length_squared() const -> float;

  [[nodiscard]]
  constexpr auto length() const -> float;
  constexpr void normalize();

  [[nodiscard]]
  constexpr auto normal() const -> Rotor3;

  // convert to matrix
  [[nodiscard]]
  constexpr auto to_mat3() const -> Mat3;

  /**
   * @param w The real component of a quaternion
   * @param i The first imaginary component of the quaternion (glm and gltf call
   * it x)
   * @param j The second imaginary component of the quaternion (glm and gltf
   * call it y)
   * @param k The third imaginary component of the quaternion (glm and gltf call
   * it z)
   */
  static constexpr auto from_quaternion(float w, float i, float j,
                                        float k) -> Rotor3
  {
    return Rotor3{w, -k, j, -i};
  }

  // Equality comparison
  [[nodiscard]]
  friend constexpr auto operator==(Rotor3 lhs, Rotor3 rhs) -> bool
  {
    return lhs.a_ == rhs.a_ && lhs.xy_ == rhs.xy_ && lhs.xz_ == rhs.xz_ &&
           lhs.yz_ == rhs.yz_;
  }
};

// @brief construct the rotor that rotates one unit vector to another
constexpr Rotor3::Rotor3(Vec3 from, Vec3 to)
{
  a_ = 1 + dot(to, from);
  // the left side of the products have b a, not a b, so flip
  const BiVec3 minusb = wedge(to, from);
  xy_ = minusb.xy;
  xz_ = minusb.xz;
  yz_ = minusb.yz;
  normalize();
}

// angle+plane, plane must be normalized
inline Rotor3::Rotor3(const BiVec3& bv_plane, Radian angle)
{
  const float sina = sin(angle / 2.f);
  a_ = cos(angle / 2.f);
  // the left side of the products have b a, not a b
  xy_ = -sina * bv_plane.xy;
  xz_ = -sina * bv_plane.xz;
  yz_ = -sina * bv_plane.yz;
}

// Rotor3-Rotor3 product
// non-optimized
constexpr auto Rotor3::operator*(const Rotor3& q) const -> Rotor3
{
  const Rotor3& p = *this;
  Rotor3 r;

  // here we just expanded the geometric product rules
  r.a_ = p.a_ * q.a_ - p.xy_ * q.xy_ - p.xz_ * q.xz_ - p.yz_ * q.yz_;
  r.xy_ = p.xy_ * q.a_ + p.a_ * q.xy_ + p.yz_ * q.xz_ - p.xz_ * q.yz_;
  r.xz_ = p.xz_ * q.a_ + p.a_ * q.xz_ - p.yz_ * q.xy_ + p.xy_ * q.yz_;
  r.yz_ = p.yz_ * q.a_ + p.a_ * q.yz_ + p.xz_ * q.xy_ - p.xy_ * q.xz_;

  return r;
}

/// R x R*
// non-optimized
constexpr auto Rotor3::rotate(const Vec3& x) const -> Vec3
{
  const Rotor3& p = *this;

  // q = P x
  Vec3 q;
  q[0] = p.a_ * x[0] + x[1] * p.xy_ + x[2] * p.xz_;
  q[1] = p.a_ * x[1] - x[0] * p.xy_ + x[2] * p.yz_;
  q[2] = p.a_ * x[2] - x[0] * p.xz_ - x[1] * p.yz_;

  const float q012 = x[0] * p.yz_ - x[1] * p.xz_ + x[2] * p.xy_; // trivector

  // r = q P*
  Vec3 r;
  r[0] = p.a_ * q[0] + q[1] * p.xy_ + q[2] * p.xz_ + q012 * p.yz_;
  r[1] = p.a_ * q[1] - q[0] * p.xy_ - q012 * p.xz_ + q[2] * p.yz_;
  r[2] = p.a_ * q[2] + q012 * p.xy_ - q[0] * p.xz_ - q[1] * p.yz_;

  // trivector part of the result is always zero!

  return r;
}

// Rotor3-Rotor3 product
constexpr auto Rotor3::operator*=(const Rotor3& r) -> Rotor3&
{
  (*this) = (*this) * r;
  return *this;
}

// rotate a rotor by another
constexpr auto Rotor3::rotate(Rotor3 r) const -> Rotor3
{
  // should unwrap this for efficiency
  return (*this) * r * (*this).reverse();
}

// Equivalent to conjugate
constexpr auto Rotor3::reverse() const -> Rotor3
{
  return Rotor3(a_, -xy_, -xz_, -yz_);
}
// Length Squared
constexpr auto Rotor3::length_squared() const -> float
{
  return a_ * a_ + xy_ * xy_ + xz_ * xz_ + yz_ * yz_;
}
// Length
constexpr auto Rotor3::length() const -> float
{
  return sqrt(length_squared());
}

// Normalize
constexpr void Rotor3::normalize()
{
  const float l = length();
  a_ /= l;
  xy_ /= l;
  xz_ /= l;
  yz_ /= l;
}
// Normalized rotor
[[nodiscard]]
constexpr auto Rotor3::normal() const -> Rotor3
{
  Rotor3 r = *this;
  r.normalize();
  return r;
}

// convert to matrix
// non-optimized
[[nodiscard]]
constexpr auto Rotor3::to_mat3() const -> Mat3
{
  const Vec3 v0 = rotate(Vec3(1, 0, 0));
  const Vec3 v1 = rotate(Vec3(0, 1, 0));
  const Vec3 v2 = rotate(Vec3(0, 0, 1));

  return Mat3(v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
}

} // namespace beyond

#endif // BEYOND_CORE_MATH_ROTOR_HPP
