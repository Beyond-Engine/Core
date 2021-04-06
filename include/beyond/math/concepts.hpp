#ifndef BEYOND_CORE_MATH_CONCEPTS_HPP
#define BEYOND_CORE_MATH_CONCEPTS_HPP

#include <concepts>

namespace beyond {

template <class T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

} // namespace beyond

#endif // BEYOND_CORE_MATH_CONCEPTS_HPP
