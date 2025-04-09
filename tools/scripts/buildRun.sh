#! /bin/bash

cd ../../;

cmake --build build;

cd build/;

./clientTest .;