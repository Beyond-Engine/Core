# Compiler specific settings

if (beyond_core_compiler_included)
    return()
endif ()
set(beyond_core_compiler_included true)

add_library(beyond_compiler_warnings INTERFACE)
add_library(beyond::compiler_warnings ALIAS beyond_compiler_warnings)

add_library(beyond_compiler_options INTERFACE)
add_library(beyond::compiler_options ALIAS beyond_compiler_options)

option(BEYOND_CORE_WARNING_AS_ERROR "Treats compiler warnings as errors" ON)
if (MSVC)
    target_compile_options(beyond_compiler_warnings INTERFACE /W4 "/permissive-")
    if (BEYOND_CORE_WARNING_AS_ERROR)
        target_compile_options(beyond_compiler_warnings INTERFACE /WX)
    endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(beyond_compiler_warnings
            INTERFACE -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wconversion
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wsign-conversion
            -Wnull-dereference
            -Wformat=2
            -Wno-float-conversion
            )
    if (BEYOND_CORE_WARNING_AS_ERROR)
        target_compile_options(beyond_compiler_warnings INTERFACE -Werror)
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(beyond_compiler_warnings
                INTERFACE -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wuseless-cast
                )
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(beyond_compiler_warnings
                INTERFACE
                -Wno-implicit-float-conversion
                )
    endif ()
endif ()

option(BEYOND_CORE_ENABLE_PCH "Enable Precompiled Headers" OFF)
if (BEYOND_CORE_ENABLE_PCH)
    target_precompile_headers(beyond_compiler_options INTERFACE
            <algorithm>
            <array>
            <vector>
            <string>
            <utility>
            <functional>
            <memory>
            <memory_resource>
            <string_view>
            <cmath>
            <cstddef>
            <cstdint>
            <type_traits>
            )
endif ()

option(BEYOND_CORE_USE_ASAN "Enable the Address Sanitizers" OFF)
if (BEYOND_CORE_USE_ASAN)
    message("Enable Address Sanitizer")
    target_compile_options(beyond_compiler_options INTERFACE
            -fsanitize=address -fno-omit-frame-pointer)
    target_link_libraries(beyond_compiler_options INTERFACE
            -fsanitize=address)
endif ()

option(BEYOND_CORE_USE_TSAN "Enable the Thread Sanitizers" OFF)
if (BEYOND_CORE_USE_TSAN)
    message("Enable Thread Sanitizer")
    target_compile_options(beyond_compiler_options INTERFACE
            -fsanitize=thread)
    target_link_libraries(beyond_compiler_options INTERFACE
            -fsanitize=thread)
endif ()

option(BEYOND_CORE_USE_MSAN "Enable the Memory Sanitizers" OFF)
if (BEYOND_CORE_USE_MSAN)
    message("Enable Memory Sanitizer")
    target_compile_options(beyond_compiler_options INTERFACE
            -fsanitize=memory -fno-omit-frame-pointer)
    target_link_libraries(beyond_compiler_options INTERFACE
            -fsanitize=memory)
endif ()

option(BEYOND_CORE_USE_UBSAN "Enable the Undefined Behavior Sanitizers" OFF)
if (BEYOND_CORE_USE_UBSAN)
    message("Enable Undefined Behavior Sanitizer")
    target_compile_options(beyond_compiler_options INTERFACE
            -fsanitize=undefined)
    target_link_libraries(beyond_compiler_options INTERFACE
            -fsanitize=undefined)
endif ()