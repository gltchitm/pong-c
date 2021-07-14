#pragma once

#include "welcome.c"

void draw_centered_message(int width, int font_size, int y, char* message, SDL_Renderer* renderer);
void draw_welcome_message(int width, int height, SDL_Renderer* renderer);
