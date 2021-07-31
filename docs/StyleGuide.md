# Style Guide

In order for consistency to be met, the code fragments in this repository will be subject to a generate guide on what 
style to use.

The style guide is mainly for naming conventions, as the actually formatting is performed by Astyle. Note the CI
workflow is designed to fail if Astyle is not run. Naming is split into 2 sections, the actual style of the names (like
CamelCase, or ALL_CAPS) and the actual names themselves (file scoping, name scoping, etc)

## Naming Convention

The names should follow the follow guidelines:

- Easy to read and convey intent (concise)
- Easy to locate

## Naming Style Conventions

It's easier to show then tell, so here are snippets of c code for each convention. The style used here can be summarised
as follows:

- structs, enums, types, are all CamelCase.
- Functions and variables are snake_case.
- Method Functions are named with their associated class (still being snake case).
- Constants are SNAKE_CASE.
- Macros follow the same rules as the above.

```c
/* Macro naming follows the convention as to what they are emulating. */

#define FOURTY 40

/* Macro functions will follow the standard function naming conventions. (see below) */
#define make_float(a) (float)a

/* Macros which do have arguments that are evaluated multiple times must be in ALL_CAPs */
#define MAX_FUNCTION(a, b) ((a) > (b)) ? (a) : (b);

/* Enum members are ALL_CAPS */
typedef enum MyEnum {
    ENUM_VAR_1,
    ENUM_VAR_2
} MyEnum_t;

/* named structs */
typedef struct MyCStruct MyCStruct_t;

struct MyCStruct {

    int my_variable; /*  */

} MyCStruct_t;

/* all variables are snake_case */
static int var_3 = 3;

/* "member functions" have the class as the first part, but still maintain snake_case */
Error_t my_c_struct_function(void);

/* free functions are snake_case */
float some_function();

```
