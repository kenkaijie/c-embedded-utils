/**
 * @file
 * @brief  Provides a stack of pointers, useful in allocation.
 */
#pragma once

#include <stddef.h>
#include "error_codes.h"

typedef struct ptr_stack ptr_stack_t;

struct ptr_stack
{
    void ** m_stack;
    size_t m_max_item_count;
    size_t m_stack_top; /**< pointer to the next slot of the stack */
};

/**
 * @brief  Initialises the pointer stack
 * 
 * @param[in] stack - The pointer to the stack
 * @param[in] stack_buffer - The buffer that represents the stack
 * @param[in] stack_item_count - number of items in the stack
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_INVALID_ARG
 */
error_t ptr_stack_init(ptr_stack_t * stack, void ** stack_buffer, size_t stack_item_count);

/**
 * @brief Deinitialises the numeric stack
 */
void ptr_stack_deinit(ptr_stack_t * stack);

/**
 * @brief Gets the remaining size of the stack.
 * 
 * @param[in] stack - The pointer to the stack
 * 
 * @returns The number of entries available
 */
size_t ptr_stack_get_remaining_count(ptr_stack_t * stack);

/**
 * @brief  Pushes a value onto the stack.
 * 
 * @param[in] stack - The pointer to the stack
 * @param[in] value - The pointer to push onto the stack
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - Stack is full
 */
error_t ptr_stack_push(ptr_stack_t * stack, void * value);

/**
 * @brief  Pops a value from the stack.
 * 
 * @param[in] stack - The pointer to the stack
 * @param[inout] value - Value popped from the stack
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NULL_POINTER
 * @retval #ERR_EMPTY - Stack has not values
 */
error_t ptr_stack_pop(ptr_stack_t * stack, void ** value);
