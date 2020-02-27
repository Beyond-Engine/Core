cmake_minimum_required(VERSION 3.14)

project(beyond_core VERSION 0.0.2 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("StandardProjectSettings")
include("CMakeDependentOption")

option(BEYOND_CORE_BUILD_TESTS "Builds the tests" ON)
CMAKE_DEPENDENT_OPTION(BEYOND_CORE_BUILD_TESTS_COVERAGE
        "Build the project with code coverage support for tests" OFF
        "BEYOND_CORE_BUILD_TESTS" OFF)
option(BEYOND_CORE_BUILD_DOCUMENTATION
        "Build the documentation for the Beyond game engine core" OFF)
option(BEYOND_CORE_WARNING_AS_ERROR "Treats compiler warnings as errors" ON)
option(BEYOND_CORE_USE_ASAN "Enable the Address Sanitizers" OFF)
option(BEYOND_CORE_USE_TSAN "Enable the Thread Sanitizers" OFF)
option(BEYOND_CORE_USE_MSAN "Enable the Memory Sanitizers" OFF)
option(BEYOND_CORE_USE_UBSAN "Enable the Undefined Behavior Sanitizers" OFF)

if (BEYOND_CORE_BUILD_DOCUMENTATION)
    include("doxygen")
endif ()
include("compiler")
include("clangformat")

# Conan package manager
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_check(VERSION 1.0.0 REQUIRED)

conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)

add_subdirectory(third-party)
add_subdirectory(src)

if (BEYOND_CORE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
