# General Guidelines

These are guidelines that can be followed when creating a new module

## User Assumptions
Not every possible failure by the user can be accounted for.
- Libraries must account for static allocation (no dynamic memory calls).
- Structs for objects are declared in the header file, this exposes the implementation but allows the users to dynamically allocate memory if they wish.
- Guarding interfaces is optional, i/e we can assume the *_init function is always called before any other function.
- Deinit is always the last call, and the user MUST call init before using any other functions.
- Deinit will always succeed, and must have a void return signature (like a destructor).
- Null return values are not needed, prefer asserts to checking nulls. If a function can take a NULL pointer, the documentation MUST mention this.
