#!/bin/bash

# This file is to be called WITHIN the dev env context. It performs this by checking the tag set by the dockerfile (CEMB_UTILS_DEV_ENV)

set -e

if [[ -z $CEMB_UTILS_DEV_ENV ]]; then
    echo "Please run this in the appropriate development environment (i.e docker, or manually set up)"
    exit 1
fi

if [[ -z $PROJECT_ROOT ]]; then
    echo "Please define PROJECT_ROOT as the absolute path to the root of this repository."
    echo "If you are already in the root folder, calling 'export PROJECT_ROOT=$(pwd)' suffices."
    exit 1
fi

ARTEFACTS_DIR=${PROJECT_ROOT}/build_temp 
BUILD_DIR=${ARTEFACTS_DIR}/build
COVERAGE_DIR=${ARTEFACTS_DIR}/coverage

# make all directories
mkdir -p ${ARTEFACTS_DIR}
mkdir -p ${BUILD_DIR}
mkdir -p ${COVERAGE_DIR}

# build and test
pushd ${BUILD_DIR}

cmake ${PROJECT_ROOT}
cmake --build .
ctest -T Test -V --no-compress-output --output-on-failure --no-tests=error

popd


#coverage
pushd ${COVERAGE_DIR}

gcovr -r ${PROJECT_ROOT} ${BUILD_DIR} --xml-pretty  --exclude-directories='.*\/extern\/.*' > coverage_report.xml
gcovr -r ${PROJECT_ROOT} ${BUILD_DIR} --html --html-details -o coverage_report.html  --exclude-directories='.*\/extern\/.*'

popd
