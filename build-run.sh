#! /bin/bash

mkdir ./build/ ;

cmake -S . -B build/ ;

cd build/ ; make install ;

cd bin/ ; ./Venus . ;