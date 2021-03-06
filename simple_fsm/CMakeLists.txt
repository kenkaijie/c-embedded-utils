set(C_EMB_MODULE_NAME simple_fsm)

set(C_EMB_MODULE_SOURCES src/simple_fsm.c)
set(C_EMB_MODULE_LINKS error_codes)
set(C_EMB_MODULE_TEST_SOURCES test/test_simple_fsm.c test/test_simple_fsm_mock_fsm.c)
set(C_EMB_MODULE_TEST_RUNNER_SOURCES test_runner/test_simple_fsm_main.c)

add_library(${C_EMB_MODULE_NAME} STATIC)
target_include_directories(${C_EMB_MODULE_NAME}  PUBLIC inc)
target_sources(${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_SOURCES})
target_link_libraries(${C_EMB_MODULE_NAME} ${C_EMB_MODULE_LINKS} cemd_global_options)

add_library(test_${C_EMB_MODULE_NAME}  STATIC)
target_include_directories(test_${C_EMB_MODULE_NAME} PUBLIC test)
target_sources(test_${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_TEST_SOURCES})
target_link_libraries(test_${C_EMB_MODULE_NAME} ${C_EMB_MODULE_NAME} cemb_cmocka cemd_global_options)

if (C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS)
    enable_testing()
    add_executable(test_${C_EMB_MODULE_NAME}_main)
    target_link_libraries(test_${C_EMB_MODULE_NAME}_main test_${C_EMB_MODULE_NAME} cemd_global_options)
    target_sources(test_${C_EMB_MODULE_NAME}_main PRIVATE ${C_EMB_MODULE_TEST_RUNNER_SOURCES})
    add_test(test_${C_EMB_MODULE_NAME} test_${C_EMB_MODULE_NAME}_main)
endif()