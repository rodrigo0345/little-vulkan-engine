#!/bin/bash

./compile-shaders.sh
cmake -S . -B build
cd build
make
cd ..

