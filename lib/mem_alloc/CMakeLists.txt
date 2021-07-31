set(MODULE_NAME mem_alloc)

set(MODULE_SOURCES src/i_pool_allocator.c src/static_pool.c)
set(MODULE_PUBLIC_LINKS error_codes collections)
set(MODULE_TEST_SOURCES test/test_i_pool_allocator.c test/test_static_pool.c test/mock_pool_allocator.c)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_mem_alloc_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    PUBLIC_LINKS ${MODULE_PUBLIC_LINKS} 
    TEST_SOURCES ${MODULE_TEST_SOURCES}
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})