#!/bin/bash

clang++ xlib_init.cpp -g -o ../build/xlib_init -lX11 -lGL -lGLU
