#ifndef BEYOND_CORE_UTILS_PANIC_HPP
#define BEYOND_CORE_UTILS_PANIC_HPP

/**
 * @file panic.hpp
 * @ingroup util
 */

#include <source_location>
#include <string_view>

namespace beyond {

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

/** @}@} */

} // namespace beyond

#endif // BEYOND_CORE_UTILS_PANIC_HPP
