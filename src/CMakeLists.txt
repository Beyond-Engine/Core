add_library(core
        ../include/beyond/assert.hpp
        ../include/beyond/expected_extra.hpp
        ../include/beyond/prelude.hpp
        ../include/beyond/copy_move.hpp
        ../include/beyond/defer.hpp
        ../include/beyond/bitflags.hpp

        utils/panic.cpp
        geometry/aabb3.cpp
        concurrency/thread_pool.cpp
)

target_sources(core
        PUBLIC
        FILE_SET CXX_MODULES FILES
        mod.cppm
        algorithm/sort_by_key.cppm
        concurrency/thread_pool.cppm
        math/constants.cppm
        math/concepts.cppm
        math/angle.cppm
        math/functions.cppm
        math/swizzle.cppm
        math/vector.cppm
        math/matrix.cppm
        math/point.cppm
        math/transform.cppm
        math/quat.cppm
        math/bivector.cppm
        math/rotor.cppm
        math/serial.cppm
        geometry/ray.cppm
        geometry/aabb3.cppm
        container/vector_interface.cppm
        container/array.cppm
        container/at_opt.cppm
        container/static_vector.cppm
        container/slot_map.cppm
        coroutines/generator.cppm
        ecs/sparse_set.cppm
        ecs/sparse_map.cppm
        types/handle.cppm
        types/in_place.cppm
        types/monostate.cppm
        types/optional.cppm
        types/optional_conversion.cppm
        types/expected.cppm
        types/unique_function.cppm
        types/unique_ptr.cppm
        types/function_ref.cppm
        types/ref.cppm
        types/typedefs.cppm
        types/zstring_view.cppm
        utils/arrow_proxy.cppm
        utils/unreachable.cppm
        utils/crtp.cppm
        utils/filesystem_utils.cppm
        utils/functional.cppm
        utils/hash.cppm
        utils/narrowing.cppm
        utils/noexcept_cast.cppm
        utils/panic.cppm
        utils/type_traits.cppm
        utils/utils.cppm
)

set(BEYOND_CORE_ASSERT_POLICY AUTO CACHE STRING "The policy of enabling
    assertion or not in beyond core.
    AUTO means follow the debug or release build setting.")
set_property(CACHE BEYOND_CORE_ASSERT_POLICY PROPERTY STRINGS AUTO ON OFF)

if (BEYOND_CORE_ASSERT_POLICY STREQUAL "AUTO")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(core PUBLIC BEYOND_CORE_ENABLE_ASSERT)
    endif ()
elseif (BEYOND_CORE_ASSERT_POLICY STREQUAL "ON")
    target_compile_definitions(core PUBLIC BEYOND_CORE_ENABLE_ASSERT)
endif ()

target_include_directories(core
        PUBLIC SYSTEM
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

find_package(Threads)

find_package(fmt CONFIG REQUIRED)
find_package(Backward CONFIG)

target_link_libraries(core
        PRIVATE
        beyond::compiler_options
        beyond::compiler_warnings
        PUBLIC
        Threads::Threads
        fmt::fmt
)
if (TARGET Backward::Backward)
    target_link_libraries(core
            PRIVATE
            Backward::Backward)
endif ()

add_library(beyond::core ALIAS core)
