#!/usr/bin/env bash

MYPWD=$(pwd)

set -x
set -e

GCC=gcc
GXX=g++

BUILD_TYPE=RelWithDebInfo

NUM_CORES=`getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu || echo 1`
# Don't use all (virtual) cores in an attempt to not freeze the system.
NUM_PARALLEL_BUILDS=$((NUM_CORES - 2 < 1 ? 1 : NUM_CORES - 2))

CXX_MARCH=native

OPENCV_DIR="$MYPWD/thirdparty/opencv"

COMMON_CMAKE_ARGS=(
    -DCMAKE_C_COMPILER=${GCC}
    -DCMAKE_CXX_COMPILER=${GXX}
    -DCMAKE_C_COMPILER_LAUNCHER=ccache
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
    -DCMAKE_CXX_FLAGS="-march=$CXX_MARCH -O3 -Wno-deprecated-declarations -Wno-null-pointer-arithmetic -Wno-unknown-warning-option -Wno-unused-function" #  -Wno-int-in-bool-context
)

BUILD_OPENCV=thirdparty/build-opencv

git submodule sync --recursive
git submodule update --init --recursive

rm -rf "$BUILD_OPENCV"

mkdir -p "$BUILD_OPENCV"
pushd "$BUILD_OPENCV"
cmake ../opencv "${COMMON_CMAKE_ARGS[@]}" \
    -DBUILD_EXAMPLES=OFF \
    -DWITH_CUDA=ON \
    -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules/ 
make -j$NUM_PARALLEL_BUILDS
popd



