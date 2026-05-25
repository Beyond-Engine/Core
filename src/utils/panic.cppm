module;

#include <source_location>
#include <string_view>

#include <fmt/format.h>

export module beyond.core:panic;

export namespace beyond {

/**
 * @addtogroup core
 * @{
 * @addtogroup util
 * @{
 */

/**
 * @brief Dumps some error messages and terminates the program
 * @param msg The error message to output before abort
 */
[[noreturn]] void
panic(std::string_view msg,
      std::source_location source_location = std::source_location::current());

/// @brief This function is usually triggered by assertion macros
/// @see BEYOND_ASSERT, BEYOND_ASSERT_MSG, BEYOND_ENSURE, BEYOND_ENSURE_MSG
[[noreturn]] void assert_failed(
    std::string_view cond_string, std::string_view msg,
    std::source_location source_location = std::source_location::current());

/** @}@} */

} // namespace beyond
