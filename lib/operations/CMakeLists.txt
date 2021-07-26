set(MODULE_NAME operations)

set(MODULE_SOURCES src/bit_ops.c src/numeric_ops.c)
set(MODULE_TEST_SOURCES test/test_bit_ops.c test/test_numeric_ops.c)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_operations_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    TEST_SOURCES ${MODULE_TEST_SOURCES}
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})