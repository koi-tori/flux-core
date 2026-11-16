#!/usr/bin/bash
set -euo pipefail

BUILD_TYPE="Debug"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"
BUILD_DIR="${PROJECT_DIR}/build"

while [ "$#" -gt 0 ]; do
    case "$1" in
        -t | --type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -r | --remove)
            rm -rf ${BUILD_DIR}
            shift
            ;;
        -h | --help)
            echo "-t --type    Change cmake build type"
            echo "-r --remove  Remove recent build files"
            echo "-h --help    Show help"
            exit 0
            ;;
        *)
            echo "Use --help to know about usage."
            exit 1
            ;;
    esac
done

if ! command -v ninja >/dev/null 2>&1; then
    echo "Ninja is needed to build fluxc."
    exit 1
fi

cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" -G Ninja \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_CXX_COMPILER="${CXX:-g++}"

cmake --build "${BUILD_DIR}"

echo "Build finished." "${BUILD_DIR}"
