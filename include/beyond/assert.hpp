#ifndef BEYOND_CORE_UTILS_ASSERT_HPP
#define BEYOND_CORE_UTILS_ASSERT_HPP

/**
 * @file assert.hpp
 * @brief Assertion macros
 * @ingroup util
 * @warning This header is not self-contained. You must `import beyond.core;`
 *          before including it
 */

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

#ifdef BEYOND_CORE_ENABLE_ASSERT
#define BEYOND_ASSERT(condition)                                               \
  do {                                                                         \
    if (!(condition)) { ::beyond::assert_failed(#condition, ""); }             \
  } while (0)

#define BEYOND_ASSERT_MSG(condition, message)                                  \
  do {                                                                         \
    if (!(condition)) { ::beyond::assert_failed(#condition, message); }        \
  } while (0)

#else
#define BEYOND_ASSERT(condition)                                               \
  do {                                                                         \
  } while (0)

#define BEYOND_ASSERT_MSG(condition, message)                                  \
  do {                                                                         \
  } while (0)

#endif

// Like BEYOND_ASSERT, but will always check the condition
#define BEYOND_ENSURE(condition)                                               \
  do {                                                                         \
    if (!(condition)) { ::beyond::assert_failed(#condition, ""); }             \
                                                                               \
  } while (0)

#define BEYOND_ENSURE_MSG(condition, message)                                  \
  do {                                                                         \
    if (!(condition)) { ::beyond::assert_failed(#condition, message); }        \
  } while (0)

/** @}
 *  @} */

#endif // BEYOND_CORE_UTILS_ASSERT_HPP
