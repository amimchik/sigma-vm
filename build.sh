#!/usr/bin/sh

source ./config.sh

mkdir build
cmake -B build -S . 
cmake --build build
