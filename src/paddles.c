const int PADDING_FROM_SIDE = 15;
const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 200;

struct Paddles {
    int left_paddle;
    int left_paddle_proposed;
    int right_paddle;
    int right_paddle_proposed;
};

void draw_paddle(int x, int y, SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void draw_paddles(struct Paddles* paddles, int width, SDL_Renderer* renderer) {
    draw_paddle(PADDING_FROM_SIDE, paddles->left_paddle, renderer);
    draw_paddle(width - PADDLE_WIDTH - PADDING_FROM_SIDE, paddles->right_paddle, renderer);
}
