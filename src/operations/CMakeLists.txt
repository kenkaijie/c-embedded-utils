set(MODULE_SOURCES bit_ops.c 
                   numeric_ops.c)
                   
target_sources(cemb PRIVATE ${MODULE_SOURCES})
