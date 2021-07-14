#!/bin/bash

gcc -Wall -pedantic src/pong.c -o build/pong -lSDL2 -lSDL2_image -lSDL2_ttf
./build/Pong