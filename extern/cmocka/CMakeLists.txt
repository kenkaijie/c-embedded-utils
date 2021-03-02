add_library(cemb_cmocka STATIC)

target_include_directories(cemb_cmocka PUBLIC include)
target_sources(cemb_cmocka PUBLIC src/cmocka.c)