# Embedded Utils For C

Just a collection of data structures and common architectural implementations I find myself using again and again on embedded systems. This repository just provides some additional rigor over the entire system. Providing a fully tested library.

# Compiling
Just compile out of source using cmake. By default on my dev environment, I do the build in a `./build` directory.

```
TODO: Better building guide.
```

# General Info

## User Assumptions
Input validation only occurs at the interface boundary. It is not required for private methods. (and discouraged)

## Thread Safety
Thread safety is NOT implied here. Most things may work but assume that nothing is thread safe unless otherwise stated. In the header or function brief.

## Object Model
This system contains a novel C object model, which mimics rudimentary OOP principles. If this approach is not beneficial to the application, the design should ensure decoupling quite easily.

## Externs
The only depedency for this system will be standard c libraries as well as the unit testing framework. It includes a very light snapshot of cmocka for use if your system does not have a unit testing framework. 

## CMAKE Options
This project uses Cmake to do library builds. CMake will produce both static libraries as well as tests if needed. The structure of all library contents involve the actual library. The test library, as well as a test runner for PC based unit testing. For on target based testing, linking to the test library will allow for running of the cmocka tests on any platform you desire.

C_EMB_UTILS_CFG_OWN_CMAKE - by enabling this switch, this project will not import it's own cmocka library.
C_EMB_UTILS_CFG_PRODUCE_UNIT_TESTS - enabling this will add the unit test executables as targets, and add them to ctest.

### CMake Targets
Each module here provides a target for the specific component. All components provide a target, as well as a test target if tests are to be run on an embedded system.


## Generate Targets
To generate targets, we use gcovr with the following command (performed within the build folder)

Gcovr can be installed using `pip install gcovr`.

```
gcovr -r .. . --html --html-details -o example-html-details.html --exclude-directories=".*\/extern\/.*"
```

This produces a html report for the system. It excludes generating coverage for extern libraries.