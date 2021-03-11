#include "mock_critical_section.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

void mock_critical_section_get_interface(critical_section_t * interface, void * context)
{
    interface->enter = mock_critical_section_enter;
    interface->exit = mock_critical_section_exit;
    interface->context = context;
}

void mock_critical_section_enter(void * context)
{
    function_called();
    check_expected_ptr(context);
}

void mock_critical_section_exit(void * context)
{
    function_called();
    check_expected_ptr(context);
}