module;

#include <concepts>

export module beyond.core:math.concepts;

export namespace beyond {

template <class T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

} // namespace beyond
