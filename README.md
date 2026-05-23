# Beyond Core

[![GCC-10 Linux](https://github.com/Beyond-Engine/Core/actions/workflows/gcc10-linux.yml/badge.svg)](https://github.com/Beyond-Engine/Core/actions/workflows/gcc10-linux.yml)

`beyond::core` is a library with a bunch of utilities that are associated with graphics and game programming. It forms
the core utilities of the [Beyond Game Engine](https://github.com/Beyond-Engine/Beyond-Game-Engine).

## Codemap

`beyond::core` contains the following C++ library components:

* [`algorithm`](src/algorithm/)
  <br /> additional STL-style algorithm library.
* [`concurrency`](src/concurrency/)
  <br /> A work-stealing thread-pool based task system
* [`coroutines`](src/coroutines/)
  <br /> C++20 coroutine supporting library.
* [`container`](src/container)
  <br /> contains additional STL-style containers.
* [`ecs`](src/ecs) (incomplete and unusable)
  <br /> contains an implementation of an entity component system.
* [`math`](src/math/)
  <br /> contains a mathematics library with a graphics focus.
* [`types`](include/beyond/types/)
  <br /> contains non-container utility types, like the
  `optional` and `expected` types with monadic extensions.
* [`utils`](src/utils/)
  <br />  contains utility and helper code.

## Usage

This library is not in a usable state to the public yet. The features are incomplete and are coded as need. I will also
make breaking changes frequently.

## Build

### CMake Options

- `BEYOND_CORE_BUILD_TESTS` (`OFF` by default) build tests
- `BEYOND_CORE_BUILD_TESTS_COVERAGE` (`OFF` by default) test coverage with `gcov` and `lcov`
- `BEYOND_CORE_BUILD_DOCUMENTATION` (`OFF` by default) build doxygen documentation
- `BEYOND_CORE_ENABLE_PCH`  (`ON` by default) precompiled header

#### Other useful options for developing purpose

- `BEYOND_CORE_WARNING_AS_ERROR` (`ON` by default) treat compiler warnings as errors
- `BEYOND_CORE_USE_ASAN` (`OFF` by default) address sanitizer
- `BEYOND_CORE_USE_TSAN` (`OFF` by default) thread sanitizer
- `BEYOND_CORE_USE_MSAN` (`OFF` by default) memory sanitizer
- `BEYOND_CORE_USE_UBSAN` (`OFF` by default) undefined behavior sanitizer
- `BEYOND_CORE_USE_CMAKE_CONAN` (`OFF` by default) get dependencies automatically by
  using [cmake-conan](https://github.com/conan-io/cmake-conan)

## Acknowledgements

- The `optional` and `expected` types are modified from [Sy Brand](https://tartanllama.xyz/)'s
  [tl::optional](https://github.com/TartanLlama/optional)
  and [tl::expected](https://github.com/TartanLlama/expected) libraries

## License

This repository is released under MIT License. See [License](file:License) for more information.
