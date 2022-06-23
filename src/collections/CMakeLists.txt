set(MODULE_SOURCES bounded_heap.c 
                   ptr_stack.c 
                   copy_queue.c 
                   circular_buffer.c 
                   fast_circular_buffer.c)

target_sources(cemb PRIVATE ${MODULE_SOURCES})
