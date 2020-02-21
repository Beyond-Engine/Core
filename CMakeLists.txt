cmake_minimum_required(VERSION 3.14)

project(beyond_core VERSION 0.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include("CMakeDependentOption")

link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

# Organize unrelated targets to clean IDE hierarchy.
set(DEPENDENCY_FOLDER "third-party")

# This will also clean up the CMake ALL_BUILD, INSTALL, RUN_TESTS and ZERO_CHECK projects.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ${DEPENDENCY_FOLDER})

# Output binary to predictable location.
set(BINARY_OUT_DIR ${CMAKE_BINARY_DIR}/bin)
set(LIB_OUT_DIR ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUT_DIR})

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_OUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIB_OUT_DIR})
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

############################################################################
# CMake Options
############################################################################
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
