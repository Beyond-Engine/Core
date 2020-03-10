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

struct Matrix2;
struct Matrix3;
struct Matrix4;

/** @}
 *  @} */

} // namespace beyond

#endif