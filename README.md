# Beyond Core
[![Build Status](https://travis-ci.org/Beyond-Engine/Core.svg?branch=master)](https://travis-ci.org/Beyond-Engine/Core)
[![Build status](https://ci.appveyor.com/api/projects/status/4fd4193gbuj65w0e?svg=true)](https://ci.appveyor.com/project/LesleyLai/core)

`beyond::core` is a library with a bunch of utilities that are associated with graphics and game programming.
It forms the core utilities of the [Beyond Game Engine](https://github.com/Beyond-Engine/Beyond-Game-Engine).

## Codemap

`beyond::core` contains the following C++ library components:

* [`concurrency`](include/beyond/concurrency/)  (incomplete and unusable)
  <br /> contains high level constructs for concurrent programming.
* [`container`](include/beyond/container)
  <br /> contains additional STL-style containers.
* [`ecs`](include/beyond/ecs) (incomplete and unusable)
    <br /> contains an implementation of an entity component system.
* [`math`](include/beyond/math/)
  <br /> contains a mathematics library with a graphics focus.
* [`types`](include/beyond/types/)
  <br /> contains non-container utility types, like an
  `optional` type with monadic extensions.
* [`random`](include/beyond/random/)
  <br />  contains random number generators and distributions with a standard `<random>`-like interface.
* [`utils`](include/beyond/utils/)
  <br />  contains utility and helper code.

## Usage
This library is not in a usable state to the public yet.
The features are incomplete and are coded as need.
I will also make breaking changes frequently.

## License
This repository is released under Mit License. See [License](file:License) for more information.
