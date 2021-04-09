#pragma once

const int BALL_RADIUS = 13;

struct Ball {
    int x;
    int y;
    int x_speed;
    int y_speed;
};

void draw_ball(struct Ball* ball, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = 0; w < BALL_RADIUS * 2; w++) {
        for (int h = 0; h < BALL_RADIUS * 2; h++) {
            int dx = BALL_RADIUS - w;
            int dy = BALL_RADIUS - h;
            if (dx * dx + dy * dy <= BALL_RADIUS * BALL_RADIUS) {
                SDL_RenderDrawPoint(renderer, ball->x + dx, ball->y + dy);
            }
        }
    }
}
