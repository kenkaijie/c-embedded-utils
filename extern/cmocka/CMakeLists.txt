add_library(cmocka STATIC)

target_include_directories(cmocka PUBLIC include)
target_sources(cmocka PUBLIC src/cmocka.c)