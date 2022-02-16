#pragma once

struct Ball {
    int x;
    int y;
    int x_velocity;
    int y_velocity;
};

void draw_ball(struct Ball ball, SDL_Renderer* renderer);
void draw_arrow(struct Ball ball, SDL_Renderer* renderer);
void randomize_ball_speed(struct Ball* ball);
