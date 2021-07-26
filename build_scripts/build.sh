#!/bin/sh
set -e

if [ -n $CEMB_UTILS_DEV_ENV ]; then
    echo "Don't run this in your docker, run the script used inside the build environment instead."
    exit 1
fi

if [ -z $PROJECT_ROOT ]; then
    echo "Please define PROJECT_ROOT as the absolute path to the root of this repository."
    echo "If you are already in the root folder, calling 'export PROJECT_ROOT=$(pwd)' suffices."
    exit 1
fi

DOCKER_PROJECT_ROOT="/usr/project"

docker build -t c-embedded-utils -f ${PROJECT_ROOT}/Dockerfile ${PROJECT_ROOT}
# bind our project root to the internal project root, then set PROJECT_ROOT as the internal
docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="${DOCKER_PROJECT_ROOT}" c-embedded-utils "export PROJECT_ROOT=${DOCKER_PROJECT_ROOT} && /bin/bash \${PROJECT_ROOT}/build_scripts/dev_env_build.sh"
