#!/bin/bash

# This file is to be called WITHIN the dev env context. It performs this by checking the tag set by the dockerfile (CEMB_UTILS_DEV_ENV)

set -e

if [[ -z $CEMB_UTILS_DEV_ENV ]]; then
    echo "Please run this in the appropriate development environment (i.e docker, or manually set up)"
    exit 1
fi

# build
mkdir -p ${PROJECT_ROOT}/build_temp

pushd ${PROJECT_ROOT}/build_temp

cmake ..
cmake --build .

#test
cd ${PROJECT_ROOT}/build_temp
ctest -T Test -V --no-compress-output --output-on-failure --no-tests=error

#coverage
cd ${PROJECT_ROOT}/build_temp
gcovr -r .. . --xml-pretty  --exclude-directories='.*\/extern\/.*' > coverage_report.xml
gcovr -r .. . --html --html-details -o ${PROJECT_ROOT}/build_temp/coverage_report.html  --exclude-directories='.*\/extern\/.*'

popd