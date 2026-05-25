#pragma once

/**
 * @file copy_move.hpp
 * @brief Helper macros to simplify copy and move constructor/assignment
 */

#define BEYOND_DELETE_COPY(classname)                                          \
  classname(const classname&) = delete;                                        \
  auto operator=(const classname&)->classname& = delete;
#define BEYOND_DELETE_MOVE(classname)                                          \
  classname(classname&&) noexcept = delete;                                    \
  auto operator=(classname&&) & noexcept -> classname& = delete;
#define BEYOND_DEFAULT_COPY(classname)                                         \
  classname(const classname&) = default;                                       \
  auto operator=(const classname&)->classname& = default;
#define BEYOND_DEFAULT_MOVE(classname)                                         \
  classname(classname&&) noexcept = default;                                   \
  auto operator=(classname&&) & noexcept -> classname& = default;
