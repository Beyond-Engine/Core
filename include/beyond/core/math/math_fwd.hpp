#pragma once

#ifndef BEYOND_CORE_MATH_FWD_HPP
#define BEYOND_CORE_MATH_FWD_HPP

/**
 * @file math_fwd.hpp
 * @brief Provides forward declaration for the math library
 * @ingroup math
 */

namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup math
 * @{
 */

struct UninitializedTag {
};

constexpr UninitializedTag uninitialized_tag;

/**
 * @brief Fix dimensional vectors, supports a lot of common vector arithmetic
 * operations
 *
 * @see Vector<T, 2>
 * @see Vector<T, 3>
 * @see Vector<T, 4>
 */
template <typename T, std::size_t size> struct Vector;
template <typename T> using Vector2 = Vector<T, 2>;
template <typename T> using Vector3 = Vector<T, 3>;
template <typename T> using Vector4 = Vector<T, 4>;
using Vec2 = Vector2<float>;
using IVec2 = Vector2<int>;
using Vec3 = Vector3<float>;
using IVec3 = Vector3<int>;
using Vec4 = Vector4<float>;
using IVec4 = Vector4<int>;

/**
 * @brief Fix dimensional points
 *
 * @see Point<T, 2>
 * @see Point<T, 3>
 * @related Vector
 */
template <typename T, std::size_t size> struct Point;
template <typename T> using Point2 = Point<T, 2>;
template <typename T> using Point3 = Point<T, 3>;
using FPoint2 = Point2<float>;
using IPoint2 = Point2<int>;
using FPoint3 = Point3<float>;
using IPoint3 = Point3<int>;

template <typename T> struct Matrix2;
template <typename T> struct Matrix3;
template <typename T> struct Matrix4;
using Mat2 = Matrix2<float>;
using Mat3 = Matrix3<float>;
using Mat4 = Matrix4<float>;

/** @}
 *  @} */

} // namespace beyond

#endif