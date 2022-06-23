#!/bin/bash -e

# This file is to be called WITHIN the dev env context. It performs this by checking the tag set by the dockerfile (CEMB_UTILS_DEV_ENV)

if [[ -z $CEMB_UTILS_DEV_ENV ]]; then
    echo "Please run this in the appropriate development environment (i.e docker, or manually set up)"
    exit 1
fi

if [[ -z $PROJECT_ROOT ]]; then
    echo "Please define PROJECT_ROOT as the absolute path to the root of this repository."
    echo "If you are already in the root folder, calling 'export PROJECT_ROOT=$(pwd)' suffices."
    exit 1
fi

ARTEFACTS_DIR="${PROJECT_ROOT}/build_temp"
BUILD_DIR="${ARTEFACTS_DIR}/build"
COVERAGE_DIR="${ARTEFACTS_DIR}/coverage"

# make all directories
mkdir -p ${ARTEFACTS_DIR}
mkdir -p ${BUILD_DIR}
mkdir -p ${COVERAGE_DIR}

# build for coverage and test
pushd ${BUILD_DIR}

export CFLAGS="--coverage -DNDEBUG"
export LDFLAGS="--coverage -lgcov"
cmake ${PROJECT_ROOT}
cmake --build . --verbose
ctest -T Test -V --no-compress-output --output-on-failure --no-tests=error

popd

#coverage
pushd ${PROJECT_ROOT}

gcovr -f src -f tests -f include --xml-pretty > "${COVERAGE_DIR}/coverage_report.xml"
gcovr -f src -f tests -f include --html --html-details -o "${COVERAGE_DIR}/coverage_report.html"

popd
