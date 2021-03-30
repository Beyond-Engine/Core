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