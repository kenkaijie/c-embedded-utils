#include "test_static_event_publisher.h"

int main(void)
{
    int result = 0;
    result |= test_static_event_publisher_run_tests();
    return result;
}