#include "ptr_stack.h"

#include "mem_utils.h"

error_t ptr_stack_init(ptr_stack_t * stack, void ** stack_buffer, size_t stack_item_count)
{
    if (stack_buffer == NULL) return ERR_NULL_POINTER;
    if (stack_item_count == 0) return ERR_INVALID_ARG;
    stack->m_stack = stack_buffer;
    stack->m_max_item_count = stack_item_count;
    stack->m_stack_top = 0;
    return ERR_NONE;
}

void ptr_stack_deinit(ptr_stack_t * stack)
{
    mem_utils_fill_deadbeef(stack, sizeof(ptr_stack_t));
}

size_t ptr_stack_get_remaining_count(ptr_stack_t * stack)
{
    return stack->m_max_item_count - stack->m_stack_top;
}

error_t ptr_stack_push(ptr_stack_t * stack, void * value)
{
    if (ptr_stack_get_remaining_count(stack) == 0) return ERR_NO_MEM;

    stack->m_stack[stack->m_stack_top] = value;
    stack->m_stack_top++;

    return ERR_NONE;
}

error_t ptr_stack_pop(ptr_stack_t * stack, void ** value)
{
    if (value == NULL) return ERR_NULL_POINTER;
    if (ptr_stack_get_remaining_count(stack) == stack->m_max_item_count) return ERR_EMPTY;

    stack->m_stack_top--;
    *value = stack->m_stack[stack->m_stack_top];
    
    return ERR_NONE;
}