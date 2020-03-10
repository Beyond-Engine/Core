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