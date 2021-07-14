#pragma once

#include "scoreboard.c"

void draw_score(int current_score, int x, int y, SDL_Renderer* renderer);
void draw_scoreboard(int left_score, int right_score, int width, SDL_Renderer* renderer);
