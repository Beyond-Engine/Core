#pragma once

#ifndef BEYOND_CORE_MATH_FWD_HPP
#define BEYOND_CORE_MATH_FWD_HPP

/**
 * @file math_fwd.hpp
 * @brief Provides forward declaration for the math library
 * @ingroup math
 */

#include <cstddef>

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
 * @see TVec<T, 2>
 * @see TVec<T, 3>
 * @see TVec<T, 4>
 */
template <typename T, std::size_t size> struct TVec;
template <typename T> using TVec2 = TVec<T, 2>;
template <typename T> using TVec3 = TVec<T, 3>;
template <typename T> using TVec4 = TVec<T, 4>;
using Vec2 = TVec2<float>;
using IVec2 = TVec2<int>;
using Vec3 = TVec3<float>;
using IVec3 = TVec3<int>;
using Vec4 = TVec4<float>;
using IVec4 = TVec4<int>;

/**
 * @brief Fix dimensional points
 *
 * @see TPoint<T, 2>
 * @see TPoint<T, 3>
 * @related TVec
 */
template <typename T, std::size_t size> struct TPoint;
template <typename T> using TPoint2 = TPoint<T, 2>;
template <typename T> using TPoint3 = TPoint<T, 3>;
using Point2 = TPoint2<float>;
using IPoint2 = TPoint2<int>;
using Point3 = TPoint3<float>;
using IPoint3 = TPoint3<int>;

template <typename T> struct TMat2;
template <typename T> struct TMat3;
template <typename T> struct TMat4;
using Mat2 = TMat2<float>;
using Mat3 = TMat3<float>;
using Mat4 = TMat4<float>;

/** @}
 *  @} */

} // namespace beyond

#endif