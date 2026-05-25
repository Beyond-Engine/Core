module;

#include <source_location>

export module beyond.core:unreachable;

import :panic;

export namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

/**
 * @brief Marks unreachable code.
 *
 * Indicates that we know execution should never reach this point in the
 * program. In debug mode, we assert this fact because it's a bug to get here.
 *
 * In release mode, we use compiler-specific built in functions to tell the
 * compiler the code can't be reached. This avoids "missing return" warnings
 * in some cases and also lets it perform some optimizations by assuming the
 * code is never reached.
 */
[[noreturn]] void
unreachable([[maybe_unused]] std::source_location source_location =
                std::source_location::current())
{
#ifdef BEYOND_CORE_ENABLE_ASSERT
  panic("Reach unreachable code", source_location);
#else
#if defined(_MSC_VER)
  __assume(0);
#elif defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
  __builtin_unreachable();
#else
  std::terminate();
#endif
#endif
}

/** @}@} */

} // namespace beyond
