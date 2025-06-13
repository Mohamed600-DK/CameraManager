#! /bin/bash
BUILD_TYPE=Release

if [ -e "build/$BUILD_TYPE" ]; then 
    rm -r build/$BUILD_TYPE/*
fi



conan install .  --build=missing -s build_type=$BUILD_TYPE

cmake -B build/$BUILD_TYPE/ -DCMAKE_TOOLCHAIN_FILE=build/$BUILD_TYPE/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE

cmake --build build/$BUILD_TYPE


./build/$BUILD_TYPE/example 