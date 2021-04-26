set(MODULE_NAME simple_fsm)

set(MODULE_SOURCES src/simple_fsm.c)
set(MODULE_LINKS error_codes)
set(MODULE_TEST_SOURCES test/test_simple_fsm.c test/mock_fsm.c)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_simple_fsm_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    LINKS ${MODULE_LINKS} 
    TEST_SOURCES ${MODULE_TEST_SOURCES} 
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})