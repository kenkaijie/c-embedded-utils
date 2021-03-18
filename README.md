# Embedded Utils For C

[![codecov](https://codecov.io/gh/kenkaijie/c-embedded-utils/branch/master/graph/badge.svg?token=WY02IOEUE0)](https://codecov.io/gh/kenkaijie/c-embedded-utils)
[![build](https://github.com/kenkaijie/c-embedded-utils/actions/workflows/ci_runner.yml/badge.svg?branch=master)](https://github.com/kenkaijie/c-embedded-utils/actions/workflows/ci_runner.yml)

Just a collection of data structures and common architectural implementations I find myself using again and again on embedded systems. This repository just provides some additional rigor over the entire system. Providing a fully tested library.

# Compiling
Compiling as per the CI platform uses docker as a primary means of abstracting the build system. Simply define the absolute path to the root of this repository as `PROJECT_ROOT` and run the `build.sh` script.

# General Info

## API Sanity Checking
Input validation only occurs at the interface boundary. It is not required for private methods. (and discouraged).

## C Limitations
One limitations for the objects is that we cannot enforce the constructor to be called. If a limited lifetime object is instantiated, it may be able to (very likely) bypass the initialisation checks provided by the system due to the random nature of the initial value. Because of this, calling any function other than the init function for an object results in undefined behaviour.

## Documentation
All library public methods should be documented in the respective header files. The current style should be maintained (see other modules for references).

Ensure that all possible return codes are accounted for, so the users can confidently know which return codes are of interest to them.

## Thread Safety
Thread safety is <b>NOT</b> implied here. Some things may work but assume that nothing is thread safe unless otherwise stated in the header or function brief. For certain modules, they will specify how to make the module thread safe with some tips, for example, wrapping functions with synchronisation primatives. This allows for extension of the functions by adding thread safe function calls.

## Externs
The only depedency for this system will be standard c libraries as well as the unit testing framework. It includes a very light snapshot of cmocka for use if your system does not have a unit testing framework. 

## CMAKE Options
This project contains options which allow for the triggering of different actions and build outcomes. Options may also in turn expose new or different targets. Refer to the `CMakeLists.txt` in the root to view the options.

### CMake Targets
Each module here provides a target for the specific component. All components provide a target, as well as a test target if tests are to be run on an embedded system.

## Generate Targets
To generate targets, we use gcovr with the following command (performed within the build folder)

Gcovr can be installed using `pip install gcovr`.

```
gcovr -r .. . --xml-pretty  --exclude-directories=".*\/extern\/.* > coverage_report.xml"
```

This produces a html report for the system. It excludes generating coverage for extern libraries.

# Module Overviews
If a module is added, ensure a small breif is placed here. The detailed functionality that cannot be explained in the function's inline documentation can fall in the module's root directory.

## Simple FSM
A basic finite state machine runner.

## Error Codes
Base implementation of commonly used error codes within this library.

## Struct Pack
A set of serialisation functions.

## Pubsub
An event publishing implementation.

## Collections
A collection of static interfaces that are used in numerous modules.

## Lock Primatives
An interface for lock primatives. Used by certain low level data structures to ensure atomicity of actions.

## Object Pool
An implementation of an object pool. 

# Guidelines
These are guidelines for coding up modules.

## Objects should have a Init and Deinit function
These acts as the constructor and the destructor. Should at the minimum return the following codes:

For Init:

- ERR_NONE - success
- ERR_NULL_POINTER - a null pointer was found
- ERR_INVALID_ARG - Config was invalid (if a unexpected NULL was found, returns ERR_NULL_POINTER instead)

For Deinit:

- ERR_NONE - success  
- ERR_NULL_POINTER - a null pointer was found   