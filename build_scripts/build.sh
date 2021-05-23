#!/bin/sh
set -e

if [ -z $PROJECT_ROOT ]; then
    echo "Please define PROJECT_ROOT as the absolute path to the root of this repository."
    echo "If you are already in the root folder, calling 'export PROJECT_ROOT=$(pwd)' suffices."
    exit 1
fi

docker build -t c-embedded-utils -f ${PROJECT_ROOT}/Dockerfile ${PROJECT_ROOT}

# Build

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "mkdir -p /usr/project/build_temp && cd /usr/project/build_temp && cmake .. && cmake --build ."

# Test

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "cd /usr/project/build_temp && ctest -T Test -V --no-compress-output --output-on-failure"

# Report

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "cd /usr/project/build_temp && gcovr -r .. . --xml-pretty  --exclude-directories='.*\/extern\/.*' > coverage_report.xml"

# HTML report

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "cd /usr/project/build_temp && gcovr -r .. . --html --html-details -o /usr/project/build_temp/coverage_report.html  --exclude-directories='.*\/extern\/.*'"