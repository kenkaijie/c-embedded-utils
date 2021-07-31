# Embedded Utils For C

<a href="https://codecov.io/gh/kenkaijie/c-embedded-utils"><img src="https://codecov.io/gh/kenkaijie/c-embedded-utils/branch/master/graph/badge.svg?token=WY02IOEUE0"></a>
<a href="https://github.com/kenkaijie/c-embedded-utils/actions/workflows/ci_runner.yml"><img src="https://github.com/kenkaijie/c-embedded-utils/actions/workflows/ci_runner.yml/badge.svg?branch=master"></a>

Just a collection of data structures and implementations I find myself using again and again on embedded systems. This 
repository just provides some additional rigor over the entire system. Providing a fully tested library for reuse.

# Dev Environment
The dev environment is described in the `Dockerfile`. This provides all the tooling to build, compile, and test the code
within this project. It is also used by the CI/CD pipeline. It is strongly recommended to do the development within this
environment, as all the tooling is already set up.

This repository therefore needs the following prerequisites as a minimum:
- Docker

This docker container can be used both as a standalone unit for compiling, or can be opened as a development environment 
for the project.

As a bare minimum, define the `PROJECT_ROOT` variable as the absolute path to the root of this directory.

```sh
# Called in shell when the current directoy is the root of this repository
export PROJECT_ROOT=$(pwd) && ${PROJECT_ROOT}/build_scripts/build.sh
```

Note: Doxygen generation output requires the environmental variable PLANTUML_JAR_PATH to be set to the absolution path
to the location of the plantuml executable. For example, `/usr/share/plantuml/plantuml.jar`. This is all setup when
running inside the dev environment.

## Automation Scripts
Most output style actions are performed with scripts in the `build_scripts` directory, including build for testing,
and documentation output.

# Other Pages
- [Style Guide](docs/StyleGuide.md): Styling information.
- [General Guidelines](docs/GeneralGuidelines.md): list of general guidelines for development, includes a list of 
assumptions we can make on the code written.
- [Module Conventions](docs/ModuleConventions.md): Information about creating new modules.
