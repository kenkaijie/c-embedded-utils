set(MODULE_NAME struct_pack)

set(MODULE_SOURCES src/struct_pack.c)
set(MODULE_LINKS "")
set(MODULE_TEST_SOURCES test/test_struct_pack.c test/test_vector3_example.c)
set(MODULE_TEST_RUNNER_SOURCES test_runner/test_struct_pack_main.c)

add_c_embedded_module(
    NAME ${MODULE_NAME} 
    SOURCES ${MODULE_SOURCES} 
    LINKS ${MODULE_LINKS} 
    TEST_SOURCES ${MODULE_TEST_SOURCES} 
    TEST_RUNNER_SOURCES ${MODULE_TEST_RUNNER_SOURCES})