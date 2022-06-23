/**
 * @file
 * @brief  Provides a stack of pointers, useful in allocation.
 */
#pragma once

#include <stddef.h>
#include "error_codes.h"

typedef struct PtrStack PtrStack_t;

/**
 * @brief A stack implementaion for storing pointers.
 * 
 * Useful as a building block for a few other higher order data structures.
 */
struct PtrStack
{
    void ** stack;
    size_t max_item_count;
    size_t stack_top; /**< pointer to the next slot of the stack */
};

/**
 * @brief  Initialises the pointer stack
 * 
 * @param[in] stack - The pointer to the stack
 * @param[in] stack_buffer - The buffer that represents the stack
 * @param[in] stack_item_count - number of items in the stack
 * 
 * @retval #ERR_NONE
 * @retval #ERR_INVALID_ARG
 * 
 * @memberof PtrStack
 */
ErrorCode_t ptr_stack_init(PtrStack_t * stack, void ** stack_buffer, size_t stack_item_count);

/**
 * @brief Deinitialises the numeric stack
 * 
 * @param[in] stack - The pointer to the stack
 * 
 * @memberof PtrStack
 */
void ptr_stack_deinit(PtrStack_t * stack);

/**
 * @brief Gets the remaining size of the stack.
 * 
 * @param[in] stack - The pointer to the stack
 * 
 * @returns The number of entries available
 * 
 * @memberof PtrStack
 */
size_t ptr_stack_get_remaining_count(PtrStack_t * stack);

/**
 * @brief  Pushes a value onto the stack.
 * 
 * @param[in] stack - The pointer to the stack
 * @param[in] value - The pointer to push onto the stack
 * 
 * @retval #ERR_NONE
 * @retval #ERR_NO_MEM - Stack is full
 * 
 * @memberof PtrStack
 */
ErrorCode_t ptr_stack_push(PtrStack_t * stack, void * value);

/**
 * @brief  Pops a value from the stack.
 * 
 * @param[in] stack - The pointer to the stack
 * @param[inout] value - Value popped from the stack
 * 
 * @retval #ERR_NONE
 * @retval #ERR_EMPTY - Stack has not values
 * 
 * @memberof PtrStack
 */
ErrorCode_t ptr_stack_pop(PtrStack_t * stack, void ** value);
