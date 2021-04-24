set(MODULE_NAME pubsub)

set(MODULE_SOURCES src/static_event_publisher.c)
set(MODULE_LINKS error_codes)
set(MODULE_TEST_SOURCES test/test_static_event_publisher.c)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_pubsub_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    LINKS ${MODULE_LINKS} 
    TEST_SOURCES ${MODULE_TEST_SOURCES} 
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})