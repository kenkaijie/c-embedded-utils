#!/bin/bash

# Runs the doxygen file generation for this project

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

DOCS_DIR=${PROJECT_ROOT}/build_temp/docs

DOXYGEN_OUTPUT_DIRECTORY=${DOCS_DIR}
doxygen ${PROJECT_ROOT}/Doxyfile
