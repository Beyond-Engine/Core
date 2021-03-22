set(TEST_TARGET_NAME ${PROJECT_NAME}_test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Catch2)

add_library(catch_main STATIC main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

add_executable(${TEST_TARGET_NAME}
        algorithms/sort_by_key_test.cpp
        concurrency/task_queue_test.cpp
        concurrency/thread_pool_test.cpp
        container/slot_map_test.cpp
        container/static_vector_test.cpp
        ecs/sparse_set_test.cpp
        ecs/sparse_map_test.cpp
        math/angle_test.cpp
        math/functions_test.cpp
        math/vector_test.cpp
        math/point_test.cpp
        math/matrix_test.cpp
        math/quat_test.cpp
        math/transform_test.cpp
        math/matrix_test_util.hpp

        utils/functional_test.cpp
        utils/function_ref_test.cpp
        utils/handle_test.cpp
        utils/make_array_test.cpp
        utils/noexcept_cast_test.cpp
        utils/size_test.cpp
        serial_test_util.hpp

        types/optional_test.cpp
        types/expected_test.cpp

        geometry/ray_test.cpp
        geometry/aabb_test.cpp

        random/xorshift32_test.cpp container/at_opt_test.cpp ../include/beyond/container/at_opt.hpp)

target_link_libraries(${TEST_TARGET_NAME}
        PRIVATE
        beyond::core
        beyond::compiler_options
        catch_main)

if (${BEYOND_CORE_BUILD_TESTS_COVERAGE})
    include("../cmake/CodeCoverage.cmake")
    APPEND_COVERAGE_COMPILER_FLAGS("-g -O0 -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LCOV_EXCLUDES '/usr/*' '*/.conan/*' '*/test/*' 'test/*' '*/src/utils/panic.cpp')
    SETUP_TARGET_FOR_COVERAGE_LCOV(
            NAME coverage
            EXECUTABLE ${TEST_TARGET_NAME}
            DEPENDENCIES ${TEST_TARGET_NAME}
    )
endif ()

if (BEYOND_CORE_ENABLE_PCH)
    target_precompile_headers(${TEST_TARGET_NAME} PRIVATE
            <catch2/catch.hpp>
            )
endif ()

add_test(NAME beyond_core_unit_test
        COMMAND "${CMAKE_BINARY_DIR}/bin/${TEST_TARGET_NAME}")
add_test(NAME beyond_core_integration_test
        COMMAND "${CMAKE_BINARY_DIR}/bin/${TEST_TARGET_NAME}" "[integration]")
