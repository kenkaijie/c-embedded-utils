#!/bin/sh

# This file is to be called WITHIN the dev env context. It performs this by checking the tag set by the dockerfile (C_EMB_UTILS_DEV_ENV)

set -e

if [ -z $C_EMB_UTILS_DEV_ENV ]; then
    echo "Please run this in the appropriate development environment (i.e docker, or manually set up)"
    exit 1
fi

# build
mkdir -p /usr/project/build_temp
cd /usr/project/build_temp
cmake ..
cmake --build .

#test
cd /usr/project/build_temp
ctest -T Test -V --no-compress-output --output-on-failure

#coverage
cd /usr/project/build_temp
gcovr -r .. . --xml-pretty  --exclude-directories='.*\/extern\/.*' > coverage_report.xml
gcovr -r .. . --html --html-details -o /usr/project/build_temp/coverage_report.html  --exclude-directories='.*\/extern\/.*'
