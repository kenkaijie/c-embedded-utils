# Module Conventions

The following convention is used when developing new modules:

1. Almost everything uses `snake_case`, except macro defines and constants, which use `SNAKE_CASE`.
2. Follow the laid out file format for consistency:
```
module_name/
├─ inc/ - the headers for this module (*.h)
├─ src/ - the source files for this module (*.c)
├─ test/ - the unit tests for this module live here (both *.c and *.h)
├─ test_runner/ - directory for executing the unit testing (usually a single *.c file)
```