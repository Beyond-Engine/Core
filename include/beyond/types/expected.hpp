#ifndef BEYOND_CORE_TYPES_EXPECTED_HPP
#define BEYOND_CORE_TYPES_EXPECTED_HPP

import beyond.core;

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

#endif // BEYOND_CORE_TYPES_EXPECTED_HPP
