cmake_minimum_required(VERSION 3.13)

project(cemb LANGUAGES C)

# Project Options
option(CEMB_CFG_OWN_CMAKE "This project uses a target cmocka as the target for internal library testing. If your own cmocka is used, cmocka will be fetched from your provided target." OFF)
option(CEMB_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)

# Inclusions should be done after options are set.

add_library(cemb)
add_library(cemb::cemb ALIAS cemb)

target_include_directories(cemb PUBLIC include)

add_library(cemd_global_options INTERFACE)

include(cmake/AddCEmbeddedModule.cmake)

add_subdirectory(extern)

if (CEMB_CFG_PRODUCE_UNIT_TESTS)
    enable_testing()
    add_library(cemb_test)
    add_library(cemb::cemb_test ALIAS cemb_test)
    target_include_directories(cemb_test PRIVATE include)
    target_link_libraries(cemb_test PRIVATE cemb::cemb cmocka::cmocka)

    add_subdirectory(tests)
    
    target_compile_options(cemd_global_options INTERFACE --coverage -Og)
    # we need NDEBUG here for the unit tests to force the asserts to go away, this is because the asserts cannot be tested
    target_compile_definitions(cemd_global_options INTERFACE NDEBUG)
    target_link_libraries(cemd_global_options INTERFACE gcov)
    
    target_link_libraries(cemb PRIVATE cemd_global_options)
    target_link_libraries(cemb_test PRIVATE cemd_global_options)
    target_link_libraries(cemb_test_runner PRIVATE cemd_global_options)

endif()

add_subdirectory(src)
