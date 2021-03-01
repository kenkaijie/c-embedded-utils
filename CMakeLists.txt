cmake_minimum_required(VERSION 3.14)

project(c_embedded_utils VERSION 0.0.0 LANGUAGES C)

option(C_EMB_UTILS_CFG_OWN_CMAKE "Provide your own cmocka target" OFF)
option(C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)

if (NOT C_EMB_UTILS_CFG_OWN_CMAKE)
add_subdirectory(extern/cmocka)
endif()

if (C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS)
enable_testing()
endif()

add_subdirectory(struct_pack)
add_subdirectory(simple_fsm)
add_subdirectory(error_codes)
