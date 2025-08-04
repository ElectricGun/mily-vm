#!/bin/bash

mkdir -p .out
g++ -O3 -o  .out/milyvm ./src/main.cpp ./src/functions.cpp ./src/executor.cpp ./src/execfuncs.cpp