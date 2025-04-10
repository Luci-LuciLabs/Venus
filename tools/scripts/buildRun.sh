#! /bin/bash

cd ../../;

rm -rf build;

mkdir build;

cmake -S . -G Ninja -B build;

cmake --build build;

cd build/;

./clientTest .;