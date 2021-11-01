#!/usr/bin/env sh

cd $(dirname "$0")
if [ ! -d build ]; then
    mkdir build
fi

gcc \
    -O3 \
    -Wall \
    -Wpedantic \
    -Werror \
    -Wformat-security \
    -Wstack-protector \
    -ftrapv \
    -fPIE \
    -fstack-protector-all \
    -fstack-clash-protection \
    -D_FORTIFY_SOURCE=2 \
    src/pong.c \
    -o build/Pong\ C \
    -lSDL2 \
    -lSDL2_image \
    -lSDL2_ttf

./build/Pong\ C
