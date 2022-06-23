#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_bsearch_iter.h"
#include <cemb/bsearch_iter.h>


static void _odd_even_help_tester(int *nums, size_t n, int numberToFind, int32_t expectedFoundIndex, int corruptionFlag)
{
    
    BsearchIterator_t iter;
    int result = bsearch_iter_init(&iter, 0, n - 1);
    assert_int_equal(0, result);

    int itercount = 0;

    while (bsearch_iter_status(&iter) == BSEARCH_CONTINUE && itercount < 33)
    {
        int32_t check_idx = bsearch_iter_get_check_index(&iter);

        assert_in_range(check_idx, 0, n - 1);

        BsearchNotification_t findStatus;

        if (nums[check_idx] == corruptionFlag)
        {
            findStatus = BSEARCH_NOTIFY_ERR_SKIP;
        }
        else if (nums[check_idx] == numberToFind)
        {
            findStatus = BSEARCH_NOTIFY_EQ;
        }
        else if (nums[check_idx] > numberToFind)
        {
            findStatus = BSEARCH_NOTIFY_LEFT;
        }
        else
        {
            findStatus = BSEARCH_NOTIFY_RIGHT;
        }

        bsearch_iter_notify(&iter, findStatus);

        itercount++;
    }

    assert_true(itercount <= 32);

    assert_int_equal(BSEARCH_DONE_FOUND, bsearch_iter_status(&iter));
    assert_int_equal(expectedFoundIndex, bsearch_iter_get_found_index(&iter));
}

static void _not_found_help_tester(int *nums, size_t n, int numberToFind, int corruptionFlag)
{
    BsearchIterator_t iter;
    int result = bsearch_iter_init(&iter, 0, n - 1);
    assert_int_equal(0, result);

    int itercount = 0;

    while (bsearch_iter_status(&iter) == BSEARCH_CONTINUE && itercount < 33)
    {
        int32_t check_idx = bsearch_iter_get_check_index(&iter);

        assert_in_range(check_idx, 0, n - 1);

        BsearchNotification_t findStatus;

        if (nums[check_idx] == corruptionFlag)
        {
            findStatus = BSEARCH_NOTIFY_ERR_SKIP;
        }
        else if (nums[check_idx] == numberToFind)
        {
            findStatus = BSEARCH_NOTIFY_EQ;
        }
        else if (nums[check_idx] > numberToFind)
        {
            findStatus = BSEARCH_NOTIFY_LEFT;
        }
        else
        {
            findStatus = BSEARCH_NOTIFY_RIGHT;
        }

        bsearch_iter_notify(&iter, findStatus);

        itercount++;
    }

    assert_true(itercount <= 32);
    assert_int_equal(BSEARCH_DONE_NOT_FOUND, bsearch_iter_status(&iter));
}

static void test_bsearch_array_odd_len_odd_idx(void **context)
{
    int nums[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int n = 13;

    int numberToFind = 3;
    int32_t expectedFoundIndex = 3;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_array_even_len_odd_idx(void **context)
{
    int nums[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int n = 12;

    int numberToFind = 3;
    int32_t expectedFoundIndex = 3;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_array_odd_len_even_idx(void **context)
{
    int nums[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int n = 13;

    int numberToFind = 8;
    int32_t expectedFoundIndex = 8;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_array_even_len_even_idx(void **context)
{
    int nums[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int n = 12;

    int numberToFind = 10;
    int32_t expectedFoundIndex = 10;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_at_start(void **context)
{
    int nums[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int n = 12;

    int numberToFind = 0;
    int32_t expectedFoundIndex = 0;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_at_end(void **context)
{
    int nums[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int n = 12;

    int numberToFind = 11;
    int32_t expectedFoundIndex = 11;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_at_end_with_corruption(void **context)
{
    int nums[12] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11};
    int n = 12;

    int numberToFind = 11;
    int32_t expectedFoundIndex = 11;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_at_start_with_corruption(void **context)
{
    int nums[12] = {11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 12};
    int n = 12;

    int numberToFind = 11;
    int32_t expectedFoundIndex = 0;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_in_mid_odd_with_corruption(void **context)
{
    int nums[12] = {0, 255, 255, 7, 255, 255, 255, 255, 255, 255, 255, 11};
    int n = 12;

    int numberToFind = 7;
    int32_t expectedFoundIndex = 3;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_in_mid_even_with_corruption(void **context)
{
    int nums[12] = {0, 255, 255, 255, 255, 255, 255, 255, 8, 255, 255, 11};
    int n = 12;

    int numberToFind = 8;
    int32_t expectedFoundIndex = 8;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_value_in_mid_even_with_corruption2(void **context)
{
    int nums[12] = {0, 255, 255, 255, 255, 255, 3, 255, 8, 255, 255, 11};
    int n = 12;

    int numberToFind = 8;
    int32_t expectedFoundIndex = 8;

    _odd_even_help_tester(nums, n, numberToFind, expectedFoundIndex, 255);
}

static void test_bsearch_full_corruption(void **context)
{
    int nums[12] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    int n = 12;

    int numberToFind = 23;

    _not_found_help_tester(nums, n, numberToFind, 255);
}

static void test_bsearch_value_not_present(void **context)
{
    int nums[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int n = 12;

    int numberToFind = 23;

    _not_found_help_tester(nums, n, numberToFind, 255);
}

int test_bsearch_iter_tests(void)
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_bsearch_array_odd_len_odd_idx),
        cmocka_unit_test(test_bsearch_array_odd_len_even_idx),
        cmocka_unit_test(test_bsearch_array_even_len_odd_idx),
        cmocka_unit_test(test_bsearch_array_even_len_even_idx),
        cmocka_unit_test(test_bsearch_value_at_start),
        cmocka_unit_test(test_bsearch_value_at_end),
        cmocka_unit_test(test_bsearch_value_at_end_with_corruption),
        cmocka_unit_test(test_bsearch_value_at_start_with_corruption),
        cmocka_unit_test(test_bsearch_value_in_mid_odd_with_corruption),
        cmocka_unit_test(test_bsearch_value_in_mid_even_with_corruption),
        cmocka_unit_test(test_bsearch_value_in_mid_even_with_corruption2),
        cmocka_unit_test(test_bsearch_full_corruption),
        cmocka_unit_test(test_bsearch_value_not_present)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
