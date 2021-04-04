#include "mock_critical_section.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

void mock_critical_section_as_critical_section(void * context, critical_section_t * interface)
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