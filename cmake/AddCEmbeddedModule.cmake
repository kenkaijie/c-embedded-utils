# Function to shortcut adding targets for a C Embedded Module
function(add_c_embedded_module)

set(oneValueArgs NAME)
set(multiValueArgs SOURCES LINKS TEST_SOURCES TEST_LINKS TEST_RUNNER_SOURCES TEST_RUNNER_LINKS)

cmake_parse_arguments(C_EMB_MODULE "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

add_library(${C_EMB_MODULE_NAME} STATIC)
target_include_directories(${C_EMB_MODULE_NAME}  PUBLIC inc)
target_sources(${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_SOURCES})
target_link_libraries(${C_EMB_MODULE_NAME} ${C_EMB_MODULE_LINKS} cemd_global_options)

if (C_EMB_UTILS_CFG_BUILD_SHARED)
    add_library(${C_EMB_MODULE_NAME}_shared SHARED)
    target_include_directories(${C_EMB_MODULE_NAME}_shared  PUBLIC inc)
    target_sources(${C_EMB_MODULE_NAME}_shared PRIVATE ${C_EMB_MODULE_SOURCES})
    target_link_libraries(${C_EMB_MODULE_NAME}_shared ${C_EMB_MODULE_LINKS} cemd_global_options)
endif()

add_library(test_${C_EMB_MODULE_NAME}  STATIC)
target_include_directories(test_${C_EMB_MODULE_NAME} PUBLIC test)
target_sources(test_${C_EMB_MODULE_NAME} PRIVATE ${C_EMB_MODULE_TEST_SOURCES})
target_link_libraries(test_${C_EMB_MODULE_NAME} ${C_EMB_MODULE_NAME} cmocka cemd_global_options ${C_EMB_MODULE_TEST_LINKS})

if (C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS)
    enable_testing()
    add_executable(test_${C_EMB_MODULE_NAME}_main)
    target_link_libraries(test_${C_EMB_MODULE_NAME}_main test_${C_EMB_MODULE_NAME} cemd_global_options ${C_EMB_MODULE_TEST_RUNNER_LINKS})
    target_sources(test_${C_EMB_MODULE_NAME}_main PRIVATE ${C_EMB_MODULE_TEST_RUNNER_SOURCES})
    add_test(test_${C_EMB_MODULE_NAME} test_${C_EMB_MODULE_NAME}_main)
endif()
endfunction()