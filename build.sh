#!/bin/bash

file="$1"

clang++ "$file".cpp -g -o ../build/"$file" -lX11 -lGL -lGLU
