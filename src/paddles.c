#include "paddles.h"

static void draw_paddle(int x, int y, SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, PADDLE_WIDTH, PADDLE_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void draw_paddles(struct Paddles paddles, int width, SDL_Renderer* renderer) {
    draw_paddle(PADDING, paddles.left_paddle, renderer);
    draw_paddle(width - PADDLE_WIDTH - PADDING, paddles.right_paddle, renderer);
}
