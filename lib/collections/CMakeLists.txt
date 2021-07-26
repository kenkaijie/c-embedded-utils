set(MODULE_NAME collections)

set(MODULE_SOURCES src/bounded_heap.c src/ptr_stack.c src/copy_queue.c src/circular_buffer.c src/fast_circular_buffer.c)
set(MODULE_PUBLIC_LINKS error_codes)
set(MODULE_PRIVATE_LINKS operations)
set(MODULE_TEST_SOURCES test/test_bounded_heap.c test/test_ptr_stack.c test/test_copy_queue.c test/test_circular_buffer.c test/test_fast_circular_buffer.c)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_collections_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    PUBLIC_LINKS ${MODULE_PUBLIC_LINKS} 
    PRIVATE_LINKS ${MODULE_PRIVATE_LINKS}
    TEST_SOURCES ${MODULE_TEST_SOURCES} 
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})