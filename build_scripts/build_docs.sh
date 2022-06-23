#!/bin/bash -e

# Runs the doxygen file generation for this project

if [[ -z "${PROJECT_ROOT}" ]]; then
    echo "Please define PROJECT_ROOT as the absolute path to the root of this repository."
    echo "If you are already in the root folder, calling 'export PROJECT_ROOT=$(pwd)' suffices."
    exit 1
fi

if [[ -z "${CEMB_UTILS_DEV_ENV}" ]]; then
    DOCKER_PROJECT_ROOT="//usr/project"
    docker build -t c-embedded-utils -f ${PROJECT_ROOT}/Dockerfile ${PROJECT_ROOT}
    docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="${DOCKER_PROJECT_ROOT}" \
    -e "PROJECT_ROOT=${DOCKER_PROJECT_ROOT}" c-embedded-utils "${DOCKER_PROJECT_ROOT}/build_scripts/build_docs.sh"
else
    DOCS_DIR=${PROJECT_ROOT}/build_temp/docs
    DOXYGEN_OUTPUT_DIRECTORY=${DOCS_DIR}
    doxygen ${PROJECT_ROOT}/Doxyfile
fi

