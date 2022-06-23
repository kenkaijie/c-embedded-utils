set(MODULE_SOURCES i_pool_allocator.c 
                   static_pool.c)

target_sources(cemb PRIVATE ${MODULE_SOURCES})
