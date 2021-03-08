set -e

docker build -t c-embedded-utils -f ${PROJECT_ROOT}/Dockerfile ${PROJECT_ROOT}

# Build

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "mkdir -p /usr/project/build-temp && cd /usr/project/build-temp && cmake .. && cmake --build ."

# Test

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "cd /usr/project/build-temp && ctest -V"

# Report

docker run --rm --mount type=bind,source="${PROJECT_ROOT}",target="/usr/project/" c-embedded-utils "cd /usr/project/build-temp && gcovr -r .. . --xml-pretty  --exclude-directories='.*\/extern\/.*' > coverage_report.xml"