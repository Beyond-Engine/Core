cmake_minimum_required(VERSION 3.15)

project(beyond_core VERSION 0.0.2 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_BINARY_DIR}")
include("cmake/StandardProjectSettings.cmake")
include("CMakeDependentOption")

option(BEYOND_CORE_BUILD_TESTS "Builds the tests" OFF)
CMAKE_DEPENDENT_OPTION(BEYOND_CORE_BUILD_TESTS_COVERAGE
        "Build the project with code coverage support for tests" OFF
        "BEYOND_CORE_BUILD_TESTS" OFF)
option(BEYOND_CORE_BUILD_DOCUMENTATION
        "Build the documentation for the Beyond game engine core" OFF)
option(BEYOND_CORE_ENABLE_TIME_TRACE "Enable compilation time profiling with -ftime-trace" OFF)

option(BEYOND_CORE_USE_CMAKE_CONAN "Use cmake-conan to get dependencies" OFF)
if (BEYOND_CORE_USE_CMAKE_CONAN)
    set(CONAN_SYSTEM_INCLUDES ON)
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)
    conan_check(VERSION 1.0.0 REQUIRED)

    conan_cmake_run(CONANFILE conanfile.py  # or relative build/conanfile.txt
            BASIC_SETUP
            GENERATORS cmake_find_package
            BUILD missing)
endif ()

if (BEYOND_CORE_BUILD_DOCUMENTATION)
    include("cmake/doxygen.cmake")
endif ()
include("cmake/compiler.cmake")

add_subdirectory(third-party)
add_subdirectory(src)

if (BEYOND_CORE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()


install(TARGETS core
        EXPORT beyond-core
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/beyond" DESTINATION include)