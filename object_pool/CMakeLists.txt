set(MODULE_NAME object_pool)

set(MODULE_SOURCES src/object_pool.c src/static_pool.c)
set(MODULE_LINKS error_codes lock_primitives)
set(MODULE_TEST_SOURCES test/test_object_pool.c test/test_static_pool.c test/mock_object_pool.c)
set(MODULE_TEST_LINKS test_lock_primitives)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_object_pool_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    LINKS ${MODULE_LINKS} 
    TEST_SOURCES ${MODULE_TEST_SOURCES}
    TEST_LINKS ${MODULE_TEST_LINKS}
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})