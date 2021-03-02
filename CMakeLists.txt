cmake_minimum_required(VERSION 3.14)

project(c_embedded_utils VERSION 0.0.0 LANGUAGES C)

option(C_EMB_UTILS_CFG_OWN_CMAKE "Provide your own cmocka target, by default we use the internally provided cmocka target (for these internal tests)" OFF)
option(C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)

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
