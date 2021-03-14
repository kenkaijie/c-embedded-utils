#include "critical_section.h"

extern inline error_t critical_section_validate_interface(critical_section_t const * interface);
extern inline error_t critical_section_enter(critical_section_t const * interface);
extern inline error_t critical_section_exit(critical_section_t const * interface);