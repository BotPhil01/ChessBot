#!/usr/bin/bash

cmake -S ../ -B cmake
cmake --build cmake
mv cmake/testsbin .
./testsbin --benchmark-samples 10

# 11.3821 seconds
# 171317 nodes
# 
