#pragma once

#include "paddles.c"

struct Paddles;

void draw_paddle(int x, int y, SDL_Renderer* renderer);
void draw_paddles(struct Paddles* paddles, int width, SDL_Renderer* renderer);
