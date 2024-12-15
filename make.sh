#!/bin/bash

# Define variables
BUILD_DIR=bin
CMAKE=cmake
BUILD_TYPE=Debug

export CC=clang 
export CXX=clang++

NPROC=$(($(nproc) + 1))

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
${CMAKE} -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
cd ..
${CMAKE} --build ${BUILD_DIR} -j$NPROC --target main