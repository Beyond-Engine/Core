set(TEST_TARGET_NAME ${PROJECT_NAME}_test)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(catch_main STATIC main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::Catch2)

add_executable(${TEST_TARGET_NAME}
        "concurrency/task_queue_test.cpp"
        "concurrency/task_system_test.cpp"
        "container/slot_map_test.cpp"
        "container/static_vector_test.cpp"
        "ecs/sparse_set_test.cpp"
        "ecs/sparse_map_test.cpp"
        "math/angle_test.cpp"
        "math/functions_test.cpp"
        "math/vector_test.cpp"
        "math/matrix_test.cpp"
        "utils/functional_test.cpp"
        "utils/function_ref_test.cpp"
        "utils/handle_test.cpp"
        "utils/named_type_test.cpp"
        serial_test_util.hpp math/transform_test.cpp math/matrix_test_util.hpp)

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

add_test(${TEST_TARGET_NAME} "${CMAKE_BINARY_DIR}/bin/${TEST_TARGET_NAME}")
