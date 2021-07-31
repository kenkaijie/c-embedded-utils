# Function to shortcut adding targets for a C Embedded Module
#
# Parameters:
#   - NAME: Required, the name of the library
#   - SOURCES: Optional, sources to the library, by default these are private
#   - PRIVATE_LINKS: Optional, private linkages
#   - PUBLIC_LINKS: Optional, public linkages
#   - TEST_SOURCES: Optional, sources for the unit test library
#   - TEST_LINKS: Optional, private links for the unit test library
#   - TEST_RUNNER_SOURCES: Optional, sources for test runner
#   - TEST_RUNNER_LINKS: Optional, Additional linkage for test runner
function(add_c_embedded_module)

    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES PRIVATE_LINKS PUBLIC_LINKS TEST_SOURCES TEST_LINKS TEST_RUNNER_SOURCES TEST_RUNNER_LINKS)

    cmake_parse_arguments(C_EMB_MODULE "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_library(${C_EMB_MODULE_NAME} STATIC)
    target_include_directories(${C_EMB_MODULE_NAME} PUBLIC inc)
    target_sources(${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_SOURCES})
    target_link_libraries(${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_PRIVATE_LINKS})
    target_link_libraries(${C_EMB_MODULE_NAME} PUBLIC ${C_EMB_MODULE_PUBLIC_LINKS} cemd_global_options)

    # add this to the aggregate library for cemb
    target_link_libraries(cemb PUBLIC ${C_EMB_MODULE_NAME})

    if (CEMB_CFG_PRODUCE_UNIT_TESTS)
        enable_testing()
        add_library(test_${C_EMB_MODULE_NAME} STATIC)
        target_include_directories(test_${C_EMB_MODULE_NAME} PUBLIC test)
        target_sources(test_${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_TEST_SOURCES})
        target_link_libraries(test_${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_NAME} cmocka::cmocka ${C_EMB_MODULE_TEST_LINKS})
        # test executors should just have coverage on by default.
        add_executable(test_${C_EMB_MODULE_NAME}_main)
        target_link_libraries(test_${C_EMB_MODULE_NAME}_main PRIVATE test_${C_EMB_MODULE_NAME} gcov ${C_EMB_MODULE_TEST_RUNNER_LINKS})
        target_sources(test_${C_EMB_MODULE_NAME}_main PRIVATE ${C_EMB_MODULE_TEST_RUNNER_SOURCES})
        add_test(test_${C_EMB_MODULE_NAME} test_${C_EMB_MODULE_NAME}_main)
    endif()
    
endfunction()