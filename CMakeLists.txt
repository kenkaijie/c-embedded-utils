cmake_minimum_required(VERSION 3.14)

project(c_embedded_utils LANGUAGES C)

# Project Options
option(C_EMB_UTILS_CFG_OWN_CMAKE "This project uses a target cemb_cmocka as the target for internal library testing. If your own cmocka is used, cemb_cmocka will simply be an alias." OFF)
option(C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)
option(C_EMB_UTILS_CFG_COMPILE_WITH_COVERAGE "Adds coverage to all targets" ON)

# all targets depend on this for customising the extra flags. This is sort of a "Global" option for all targets.
add_library(cemd_global_options INTERFACE)
if (C_EMB_UTILS_CFG_COMPILE_WITH_COVERAGE)
    target_compile_options(cemd_global_options INTERFACE --coverage -O0)
    target_link_libraries(cemd_global_options INTERFACE gcov)
endif()

if (NOT C_EMB_UTILS_CFG_OWN_CMAKE)
    add_subdirectory(extern/cmocka)
else()
    add_library(cemb_cmocka ALIAS cmocka)
endif()

if (C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS)
    enable_testing()
endif()

add_subdirectory(struct_pack)
add_subdirectory(simple_fsm)
add_subdirectory(error_codes)
