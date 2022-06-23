#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <cemb/ptr_stack.h>
#include "test_ptr_stack.h"

static void test_bad_init(void ** state)
{
    (void)state;

    PtrStack_t stack;
    void * stack_buffer[15];
    ErrorCode_t ret;

    ret = ptr_stack_init(&stack, stack_buffer, 0);
    assert_int_equal(ERR_INVALID_ARG, ret);
}

static void test_insert(void ** state)
{
    (void)state;
    
    PtrStack_t stack;
    void * stack_buffer[4];
    size_t stack_item_count = 4;
    ErrorCode_t ret;
    void * value;

    ret = ptr_stack_init(&stack, stack_buffer, stack_item_count);
    assert_int_equal(ERR_NONE, ret);

    size_t remaining_count = ptr_stack_get_remaining_count(&stack);
    assert_int_equal(stack_item_count, remaining_count);

    ret = ptr_stack_pop(&stack, &value);
    assert_int_equal(ERR_EMPTY, ret);

    ret = ptr_stack_push(&stack, (void *)0xBEEF);
    assert_int_equal(ERR_NONE, ret);
    ret = ptr_stack_push(&stack, (void *)0xDEAD);
    assert_int_equal(ERR_NONE, ret);
    ret = ptr_stack_push(&stack, (void *)0xCAFE);
    assert_int_equal(ERR_NONE, ret);
    ret = ptr_stack_push(&stack, (void *)0xFEED);
    assert_int_equal(ERR_NONE, ret);

    remaining_count = ptr_stack_get_remaining_count(&stack);
    assert_int_equal(0, remaining_count);

    ret = ptr_stack_push(&stack, (void *)0xDEADBEEF);
    assert_int_equal(ERR_NO_MEM, ret);

    ret = ptr_stack_pop(&stack, &value);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void *)0xFEED, value);
    ret = ptr_stack_pop(&stack, &value);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void *)0xCAFE, value);
    ret = ptr_stack_pop(&stack, &value);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void *)0xDEAD, value);
    ret = ptr_stack_pop(&stack, &value);
    assert_int_equal(ERR_NONE, ret);
    assert_ptr_equal((void *)0xBEEF, value);

    remaining_count = ptr_stack_get_remaining_count(&stack);
    assert_int_equal(stack_item_count, remaining_count);

    ptr_stack_deinit(&stack);

}

int test_ptr_stack_run_tests(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bad_init),
        cmocka_unit_test(test_insert),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}