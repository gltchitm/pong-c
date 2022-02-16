#pragma once

struct Paddles {
    int left_paddle;
    int left_paddle_proposed;
    int right_paddle;
    int right_paddle_proposed;
};

void draw_paddles(struct Paddles paddles, int width, SDL_Renderer* renderer);
