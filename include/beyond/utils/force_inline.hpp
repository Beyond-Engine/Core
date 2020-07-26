#ifndef BEYOND_CORE_UTILS_FORCE_INLINE_HPP
#define BEYOND_CORE_UTILS_FORCE_INLINE_HPP

#if _MSC_VER
#define BEYOND_FORCE_INLINE __forceinline
#else
#define BEYOND_FORCE_INLINE inline __attribute__((always_inline))
#endif

#endif // BEYOND_CORE_UTILS_FORCE_INLINE_HPP
