#!/bin/sh
set -e

if [ -z $PROJECT_ROOT ]; then
    echo "Please define PROJECT_ROOT as the absolute path to the root of this repository."
    echo "If you are already in the root folder, calling 'export PROJECT_ROOT=$(pwd)' suffices."
    exit 1
fi

docker build -t c-embedded-utils -f ${PROJECT_ROOT}/Dockerfile ${PROJECT_ROOT}
docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "/bin/sh /usr/project/build_scripts/dev_env_build.sh"
