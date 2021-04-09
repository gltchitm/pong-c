#!/bin/bash

gcc -Wall -pedantic pong.c -o build/Pong -lSDL2 -lSDL2_image -lSDL2_ttf
./build/Pong