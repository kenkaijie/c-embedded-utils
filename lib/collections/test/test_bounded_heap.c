#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_bounded_heap.h"
#include "bounded_heap.h"

#define HEAP_ITEM_COUNT (10)

/**
 *  @brief  For a max heap (root is max), we need to trigger a swap if the parent if less than the child. 
 *          Also for this test, we will use the heap to store simple numbers (not actual pointers).
 */
static bool max_heap_compare(void const * const parent, void const * const child)
{
    return (intptr_t) parent < (intptr_t) child;
}

static void test_nulls(void ** state)
{
    error_t ret;
    void * heap_storage[HEAP_ITEM_COUNT];
    bounded_heap_t heap;

    ret = bounded_heap_init(&heap, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = bounded_heap_pop(&heap, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

    ret = bounded_heap_peek(&heap, NULL);
    assert_int_equal(ERR_NULL_POINTER, ret);

}

static void test_bad_init(void ** state)
{
    error_t ret;
    void * heap_storage[HEAP_ITEM_COUNT];
    bounded_heap_t heap;
    bounded_heap_config_t heap_cfg = {
        .heap_storage = heap_storage,
        .element_count = HEAP_ITEM_COUNT,
        .compare = max_heap_compare
    };

    heap_cfg.heap_storage = NULL;
    ret = bounded_heap_init(&heap, &heap_cfg);
    assert_int_equal(ERR_NULL_POINTER, ret);
    heap_cfg.heap_storage = heap_storage;

    heap_cfg.compare = NULL;
    ret = bounded_heap_init(&heap, &heap_cfg);
    assert_int_equal(ERR_NULL_POINTER, ret);
    heap_cfg.compare = max_heap_compare;
    
    heap_cfg.element_count = 0;
    ret = bounded_heap_init(&heap, &heap_cfg);
    assert_int_equal(ERR_INVALID_ARG, ret);
    heap_cfg.element_count = HEAP_ITEM_COUNT;
}

/**
 * @brief   Test that a de initialised heap will always return failure values for each API call
 */ 
static void test_safe_deinit(void ** state)
{
    error_t ret;
    void * heap_storage[HEAP_ITEM_COUNT];
    bounded_heap_t heap;
    bounded_heap_config_t heap_cfg = {
        .heap_storage = heap_storage,
        .element_count = HEAP_ITEM_COUNT,
        .compare = max_heap_compare
    };

    ret = bounded_heap_init(&heap, &heap_cfg);
    assert_int_equal(ERR_NONE, ret);

    bounded_heap_deinit(&heap);

    void * dummy = NULL;

    ret = bounded_heap_push(&heap, dummy);
    assert_int_equal(ERR_NO_MEM, ret);

    ret = bounded_heap_peek(&heap, &dummy);
    assert_int_equal(ERR_EMPTY, ret);

    ret = bounded_heap_pop(&heap, &dummy);
    assert_int_equal(ERR_EMPTY, ret);
}

/**
 *  @brief  Generic scaffold to test the heap implementation here. So we can easily test different input orders without too much duplication.
 */
static void test_max_heap_test_scaffold(void ** values, void ** heap_storage, void * min_value, size_t item_count)
{
    error_t ret;
    bounded_heap_t heap;
    bounded_heap_config_t heap_cfg = {
        .heap_storage = heap_storage,
        .element_count = item_count,
        .compare = max_heap_compare
    };

    ret = bounded_heap_init(&heap, &heap_cfg);
    assert_int_equal(ERR_NONE, ret);

    // cant peek an empty heap
    void * dummy_peek;
    ret = bounded_heap_peek(&heap, &dummy_peek);
    assert_int_equal(ERR_EMPTY, ret);

    void * compare_check = min_value;
    // place all values in the heap, ensure that each time the peek displays the maximum value in the heap, i.e the peek value
    // is monotonically increasing.
    for (size_t idx = 0; idx < HEAP_ITEM_COUNT; ++idx)
    {
        void * peek_value;
        ret = bounded_heap_push(&heap, values[idx]);
        assert_int_equal(ERR_NONE, ret);

        ret = bounded_heap_peek(&heap, &peek_value);
        assert_int_equal(ERR_NONE, ret);

        assert_true(peek_value >= compare_check);
        compare_check = peek_value;

        // check the sizes are correct, size should increase by 1 each push
        size_t heap_count = bounded_heap_get_size(&heap);
        assert_int_equal(idx+1, heap_count);
    }

    // cant push to a full heap
    ret = bounded_heap_push(&heap, (void *)0xDEADBEEF);
    assert_int_equal(ERR_NO_MEM, ret);

    // note at this point the compare check value is at it's max
    // we then check if the pop returns the values in monotonically decreasing fashion
    for (size_t idx = 0; idx < HEAP_ITEM_COUNT; ++idx)
    {
        void * pop_value;
        ret = bounded_heap_pop(&heap, &pop_value);
        assert_int_equal(ERR_NONE, ret);

        assert_true(pop_value <= compare_check);
        compare_check = pop_value;

        // here we check that the remaining items grows by 1 each pop
        size_t heap_remaining = bounded_heap_get_remaining(&heap);
        assert_int_equal(idx+1, heap_remaining);
    }

    // cant pop an empty heap
    void * dummy_pop;
    ret = bounded_heap_pop(&heap, &dummy_pop);
    assert_int_equal(ERR_EMPTY, ret);
}


static void test_ascending_max_heap(void ** state)
{
    void * values[HEAP_ITEM_COUNT] = {(void *)0, (void *)1, (void *)2, (void *)3, (void *)4, (void *)5, (void *)6, (void *)7, (void *)8, (void *)9};
    void * min_value = 0;
    void * heap_storage[HEAP_ITEM_COUNT];
    test_max_heap_test_scaffold(values, heap_storage, min_value, HEAP_ITEM_COUNT);
}

static void test_descending_max_heap(void ** state)
{
    void * values[HEAP_ITEM_COUNT] = {(void *)9, (void *)8, (void *)7, (void *)6, (void *)5, (void *)4, (void *)3, (void *)2, (void *)1, (void *)0};
    void * min_value = 0;
    void * heap_storage[HEAP_ITEM_COUNT];
    test_max_heap_test_scaffold(values, heap_storage, min_value, HEAP_ITEM_COUNT);
}

static void test_random_order_max_heap(void ** state)
{
    void * values[HEAP_ITEM_COUNT] = {(void *)1230, (void *)5, (void *)99, (void *)3, (void *)0, (void *)6, (void *)22, (void *)0, (void *)5, (void *)5};
    void * min_value = 0;
    void * heap_storage[HEAP_ITEM_COUNT];
    test_max_heap_test_scaffold(values, heap_storage, min_value, HEAP_ITEM_COUNT);
}


int test_bounded_heap_run_tests(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_nulls),
        cmocka_unit_test(test_bad_init),
        cmocka_unit_test(test_safe_deinit),
        cmocka_unit_test(test_ascending_max_heap),
        cmocka_unit_test(test_descending_max_heap),
        cmocka_unit_test(test_random_order_max_heap),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}