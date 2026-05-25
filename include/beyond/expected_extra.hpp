#pragma once

/**
 * @file expected_extra.hpp
 * @brief Macros for working with beyond::expected.
 * @warning This header is not self-contained. You must `import beyond.core;`
 *          before including it
 */

/**
 * @brief A helper macro to make a common use case of expected more convenient
 * @remark You should prefer monadic operations of expected over this macro
 *
 * Evaluates the expression. If it evaluates to an unexpected error, return.
 * Otherwise assign the value of the result into a variable `name` with type
 * `type`.
 */
#define BEYOND_EXPECTED_ASSIGN(type, name, expr)                               \
  auto name##_ret = (expr);                                                    \
  if (!name##_ret.has_value()) {                                               \
    return beyond::make_unexpected(name##_ret.error());                        \
  }                                                                            \
  type name = std::forward<decltype(name##_ret)>(name##_ret).value()
