#pragma once

/**
 *  @file  A mock of a critical section implementation. We are testing to ensure the interface code does correctly dispatch calls.
 */

#include "critical_section.h"
#include <stdbool.h>

/**
 *  @brief  Gets a preconfigured interface for the mock. Note typically applications will also not specify a context explicitly, as they will be implicitly added.
 */
void mock_critical_section_as_critical_section(void * context, critical_section_t * interface);

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
#define _setup_mock_critical_section_enter_with_count(with_context_val, num_times) \
do { \
    expect_value_count(mock_critical_section_enter, context, (uintptr_t)with_context_val, num_times); \
} while(false)

#define _setup_mock_critical_section_exit_with_count(with_context_val, num_times) \
do { \
    expect_value_count(mock_critical_section_exit, context, (uintptr_t)with_context_val, num_times); \
} while(false)
