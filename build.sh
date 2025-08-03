#!/bin/bash

mkdir -p .out
g++ -o .out/milyvm ./src/main.cpp ./src/functions.cpp ./src/executor.cpp