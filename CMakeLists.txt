cmake_minimum_required(VERSION 3.13)

project(cemb LANGUAGES C)

# Project Options
option(CEMB_CFG_OWN_CMAKE "This project uses a target cmocka as the target for internal library testing. If your own cmocka is used, cmocka will be fetched from your provided target." OFF)
option(CEMB_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)

# Inclusions should be done after options are set.

add_library(cemb)
add_library(cemb::cemb ALIAS cemb)
add_subdirectory(cemb)

add_library(cemd_global_options INTERFACE)

include(cmake/AddCEmbeddedModule.cmake)

if (CEMB_CFG_PRODUCE_UNIT_TESTS)
    target_compile_options(cemd_global_options INTERFACE --coverage -O0 -Wall -Wextra -Werror)
    # we need NDEBUG here for the unit tests to force the asserts to go away, this is because the asserts cannot be tested
    target_compile_definitions(cemd_global_options INTERFACE NDEBUG)
    target_link_libraries(cemd_global_options INTERFACE gcov)
    enable_testing()
endif()

add_subdirectory(extern)

add_subdirectory(lib)
