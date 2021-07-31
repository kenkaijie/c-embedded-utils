#include "ptr_stack.h"

#include <assert.h>

ErrorCode_t ptr_stack_init(PtrStack_t * stack, void ** stack_buffer, size_t stack_item_count)
{
    assert(stack);
    assert(stack_buffer);

    if (stack_item_count == 0) return ERR_INVALID_ARG;
    stack->stack = stack_buffer;
    stack->max_item_count = stack_item_count;
    stack->stack_top = 0;
    return ERR_NONE;
}

void ptr_stack_deinit(PtrStack_t * stack)
{
    assert(stack);

    stack->max_item_count = 0;
    stack->stack_top = 0;
}

size_t ptr_stack_get_remaining_count(PtrStack_t * stack)
{
    assert(stack);

    return stack->max_item_count - stack->stack_top;
}

ErrorCode_t ptr_stack_push(PtrStack_t * stack, void * value)
{
    assert(stack);

    if (ptr_stack_get_remaining_count(stack) == 0) return ERR_NO_MEM;

    stack->stack[stack->stack_top] = value;
    stack->stack_top++;

    return ERR_NONE;
}

ErrorCode_t ptr_stack_pop(PtrStack_t * stack, void ** value)
{
    assert(stack);
    assert(value);

    if (ptr_stack_get_remaining_count(stack) == stack->max_item_count) return ERR_EMPTY;

    stack->stack_top--;
    *value = stack->stack[stack->stack_top];
    
    return ERR_NONE;
}