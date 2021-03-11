#pragma once

/**
 *  @file  A mock of a critical section implementation. We are testing to ensure the interface code does correctly dispatch calls.
 */

#include "critical_section.h"
#include <stdbool.h>

/**
 *  @brief  Gets a preconfigured interface for the mock. Note typically applications will also not specify a context explicitl, as they will be implicitly added.
 */
error_t mock_critical_section_get_interface(critical_section_t * interface, void * context);

/**
 *  @brief  The mocked section enter function. This usually is private but is set to public to ensure testability.
 * 
 *  @param[in] context - The passed in context
 */
void mock_critical_section_enter(void * context);

/**
 *  @brief  The mocked section exit function. This usually is private but is set to public to ensure testability.
 * 
 *  @param[in] context - The passed in context
 */
void mock_critical_section_exit(void * context);

/**
 *  @brief  Helper function to setup mocks.
 */
#define mock_critical_section_setup_entry_exit_mock_with_count(function, with_context_val, num_times) \
do { \
    expect_value_count(function, context, (uintptr_t)with_context_val, num_times); \
} while(false)
