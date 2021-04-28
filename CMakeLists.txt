cmake_minimum_required(VERSION 3.13)

project(c_embedded_utils LANGUAGES C)

# Project Options
option(C_EMB_UTILS_CFG_OWN_CMAKE "This project uses a target cmocka as the target for internal library testing. If your own cmocka is used, cmocka will be fetched from your provided target." OFF)
option(C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)
option(C_EMB_UTILS_CFG_COMPILE_WITH_COVERAGE "Adds coverage to all targets" ON)
option(C_EMB_UTILS_CFG_BUILD_SHARED "Creates the shared library target, note static libs are always built." OFF)

# Inclusions should be done after options are set.

include(cmake/AddCEmbeddedModule.cmake)

# all targets depend on this for customising the extra flags. This is sort of a "Global" option for all targets.
add_library(cemd_global_options INTERFACE)
if (C_EMB_UTILS_CFG_COMPILE_WITH_COVERAGE)
    target_compile_options(cemd_global_options INTERFACE --coverage -O0)
    target_link_libraries(cemd_global_options INTERFACE gcov)
endif()

if (C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS)
    enable_testing()
endif()

add_subdirectory(extern)

# Add Modules here

add_subdirectory(struct_pack)
add_subdirectory(simple_fsm)
add_subdirectory(error_codes)
add_subdirectory(lock_primitives)
add_subdirectory(mem_alloc)
add_subdirectory(pubsub)
add_subdirectory(collections)