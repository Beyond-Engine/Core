cmake_minimum_required(VERSION 3.15)

project(beyond_core VERSION 0.0.2 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("cmake/StandardProjectSettings.cmake")
include("CMakeDependentOption")

option(BEYOND_CORE_BUILD_TESTS "Builds the tests" OFF)
CMAKE_DEPENDENT_OPTION(BEYOND_CORE_BUILD_TESTS_COVERAGE
        "Build the project with code coverage support for tests" OFF
        "BEYOND_CORE_BUILD_TESTS" OFF)
option(BEYOND_CORE_BUILD_DOCUMENTATION
        "Build the documentation for the Beyond game engine core" OFF)

if (BEYOND_CORE_BUILD_DOCUMENTATION)
    include("cmake/doxygen.cmake")
endif ()
include("cmake/compiler.cmake")
include("cmake/clangformat.cmake")

# Conan package manager
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
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
endif ()


add_subdirectory(third-party)
add_subdirectory(src)

if (BEYOND_CORE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()
