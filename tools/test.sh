#!/usr/bin/bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"
EXECUTABLE="${BUILD_DIR}/fluxc"
TESTFILE="${PROJECT_DIR}/testfile/test.fl"

bash "${PROJECT_DIR}/tools/build.sh"
"${EXECUTABLE}" "${TESTFILE}"
