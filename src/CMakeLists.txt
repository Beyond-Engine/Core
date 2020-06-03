add_library(core
        ../include/beyond/concurrency/task_queue.hpp
        ../include/beyond/container/static_vector.hpp
        ../include/beyond/ecs/sparse_map.hpp
        ../include/beyond/ecs/sparse_set.hpp
        ../include/beyond/utils/assert.hpp
        ../include/beyond/utils/arrow_proxy.hpp
        ../include/beyond/utils/bit_cast.hpp
        ../include/beyond/utils/crtp.hpp
        ../include/beyond/utils/functional.hpp
        ../include/beyond/utils/function_ref.hpp
        ../include/beyond/utils/handle.hpp
        ../include/beyond/utils/make_array.hpp
        ../include/beyond/utils/numeric_aliases.hpp
        ../include/beyond/utils/named_type.hpp
        ../include/beyond/utils/panic.hpp
        utils/panic.cpp
        ../include/beyond/utils/type_traits.hpp
        ../include/beyond/math/angle.hpp
        ../include/beyond/math/constants.hpp
        ../include/beyond/math/vector.hpp
        ../include/beyond/math/detail/swizzle.hpp
        ../include/beyond/math/serial.hpp
        ../include/beyond/math/matrix.hpp
        ../include/beyond/math/math.hpp
        ../include/beyond/math/math_fwd.hpp
        ../include/beyond/math/transform.hpp
        ../include/beyond/math/point.hpp

        ../include/beyond/geometry/ray.hpp
        ../include/beyond/geometry/aabb3.hpp
        geometry/aabb3.cpp

        ../include/beyond/random/generators/xorshift32.hpp

        ../include/beyond/types/optional.hpp
        ../include/beyond/types/expected.hpp
        ../include/beyond/types/in_place.hpp
        ../include/beyond/types/monostate.hpp
        ../include/beyond/math/quat.hpp)

set(BEYOND_CORE_ASSERT_POLICY AUTO CACHE STRING "The policy of enabling
    assertion or not in beyond game engine core.
    AUTO means follow the debug or release build setting.")
set_property(CACHE BEYOND_CORE_ASSERT_POLICY PROPERTY STRINGS AUTO ON OFF)

if (BEYOND_CORE_ASSERT_POLICY STREQUAL "AUTO")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(core PUBLIC BEYOND_GAME_ENGINE_CORE_ENABLE_ASSERT)
    endif ()
elseif (BEYOND_CORE_ASSERT_POLICY STREQUAL "ON")
    target_compile_definitions(core PUBLIC BEYOND_GAME_ENGINE_CORE_ENABLE_ASSERT)
endif ()

target_include_directories(core
        PUBLIC SYSTEM
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        )

find_package(Threads)
target_link_libraries(core
        PRIVATE
        beyond::compiler_options
        beyond::compiler_warnings
        PUBLIC
        Threads::Threads
        CONAN_PKG::fmt
        CONAN_PKG::gsl_microsoft
        unique_function
        jthread
        )
add_library(beyond::core ALIAS core)
