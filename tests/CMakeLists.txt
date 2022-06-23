set(MODULE_SOURCES mock_fsm.c
                   mock_pool_allocator.c 
                   test_bit_ops.c
                   test_bounded_heap.c
                   test_bsearch_iter.c
                   test_circular_buffer.c
                   test_copy_queue.c
                   test_fast_circular_buffer.c
                   test_i_pool_allocator.c
                   test_le_pack.c
                   test_numeric_ops.c
                   test_pack.c
                   test_ptr_stack.c
                   test_simple_fsm.c
                   test_static_event_publisher.c
                   test_static_pool.c)

set(MODULE_TEST_RUNNER_SOURCES test_runner.c)

target_sources(cemb_test PRIVATE ${MODULE_SOURCES})
target_include_directories(cemb_test PUBLIC test)

add_executable(cemb_test_runner)
target_link_libraries(cemb_test_runner PRIVATE cemb::cemb cemb::cemb_test)
target_sources(cemb_test_runner PRIVATE ${MODULE_TEST_RUNNER_SOURCES})
add_test(test_cemb cemb_test_runner)
