#!/bin/bash

cd $(dirname "$0")
if [ ! -d build ]; then
    mkdir build
fi
gcc -Wall -pedantic src/pong.c -o build/pong -lSDL2 -lSDL2_image -lSDL2_ttf
./build/pong