#pragma once

/**
 * @file prelude.hpp
 * @brief Some of the commonly used macros
 * @ingroup util
 */

#include <type_traits>

#ifdef DOXYGEN_SHOULD_SKIP_THIS
#define BEYOND_FORCE_INLINE
#elif _MSC_VER
#define BEYOND_FORCE_INLINE __forceinline
#else
#define BEYOND_FORCE_INLINE inline __attribute__((always_inline))
#endif

#define BEYOND_MOV(...)                                                        \
  static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define BEYOND_FWD(...) static_cast<decltype(__VA_ARGS__)>(__VA_ARGS__)
