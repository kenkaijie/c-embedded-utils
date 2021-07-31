#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_static_event_publisher.h"
#include "static_event_publisher.h"

static void publisher_callback_a(void * context, void const * event_data)
{
    function_called();
    check_expected_ptr(event_data);
    check_expected_ptr(context);
}

static void publisher_callback_b(void * context, void const * event_data)
{
    function_called();
    check_expected_ptr(event_data);
    check_expected_ptr(context);
}

static void test_bad_init(void ** state)
{
    ErrorCode_t ret;
    StaticEventPublisher_t pub;
    StaticEventPublisherEntry_t subscriber = {
        .context = (void *) 0xDEADBEEFUL,
        .callback = publisher_callback_a,
    };
    StaticEventPublisherConfig_t pub_cfg = {
        .subscriber_count = 0,
        .subscribers = &subscriber,
    };

    ret = static_event_publisher_init(&pub, &pub_cfg);
    assert_int_equal(ret, ERR_INVALID_ARG);
}

static void test_publisher(void ** state)
{
    ErrorCode_t ret;
    StaticEventPublisher_t pub;
    int int_context = 5;
    int int_event = 2;
    StaticEventPublisherEntry_t subscribers[] = {
        {
        .context = (void *) &int_context,
        .callback = publisher_callback_a,
        },
        {
        .context = (void *) &int_context,
        .callback = publisher_callback_b,
        },
    };
    StaticEventPublisherConfig_t pub_cfg = {
        .subscriber_count = 2,
        .subscribers = &subscribers[0],
    };
    

    // setup mocked calls
    expect_value_count(publisher_callback_a, event_data, (uintptr_t)&int_event, 1);
    expect_value_count(publisher_callback_a, context, (uintptr_t)&int_context, 1);
    expect_value_count(publisher_callback_b, event_data, (uintptr_t)&int_event, 1);
    expect_value_count(publisher_callback_b, context, (uintptr_t)&int_context, 1);

    // setup call ordering
    expect_function_call(publisher_callback_a);
    expect_function_call(publisher_callback_b);

    ret = static_event_publisher_init(&pub, &pub_cfg);
    assert_int_equal(ret, ERR_NONE);

    static_event_publisher_notify_subscribers(&pub, &int_event);

    static_event_publisher_deinit(&pub);

}

static void test_safe_deinit(void ** state)
{
     ErrorCode_t ret;
    StaticEventPublisher_t pub;
    int int_context = 5;
    int int_event = 2;
    StaticEventPublisherEntry_t subscriber = {
        .context = (void *) &int_context,
        .callback = publisher_callback_a,
    };
    StaticEventPublisherConfig_t pub_cfg = {
        .subscriber_count = 2,
        .subscribers = &subscriber,
    };
    
    ret = static_event_publisher_init(&pub, &pub_cfg);
    assert_int_equal(ret, ERR_NONE);

    static_event_publisher_deinit(&pub);

    // by the nature of cmocka, if the function is called, an error is thrown because we do not expect it.
    static_event_publisher_notify_subscribers(&pub, &int_event);

}

int test_static_event_publisher_run_tests(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bad_init),
        cmocka_unit_test(test_publisher),
        cmocka_unit_test(test_safe_deinit),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}