#pragma once

struct Ball {
    int x;
    int y;
    int x_velocity;
    int y_velocity;
};

void draw_ball(struct Ball ball, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = 0; w < BALL_RADIUS * 2; w++) {
        for (int h = 0; h < BALL_RADIUS * 2; h++) {
            int dx = BALL_RADIUS - w;
            int dy = BALL_RADIUS - h;
            if (dx * dx + dy * dy <= BALL_RADIUS * BALL_RADIUS) {
                SDL_RenderDrawPoint(renderer, ball.x + dx, ball.y + dy);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void draw_arrow(struct Ball ball, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int center = WINDOW_HEIGHT / 2 - BALL_RADIUS / 2;

    if (ball.x_velocity > 0 && ball.y_velocity < 0) {
        SDL_RenderDrawLine(
            renderer,
            center + BALL_RADIUS,
            center - BALL_RADIUS,
            center + BALL_RADIUS + ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center + BALL_RADIUS,
            center - BALL_RADIUS - ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center + BALL_RADIUS + ARROW_SIZE,
            center - BALL_RADIUS,
            center + BALL_RADIUS + ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE
        );
    }

    if (ball.x_velocity > 0 && ball.y_velocity > 0) {
        SDL_RenderDrawLine(
            renderer,
            center + BALL_RADIUS,
            center + BALL_RADIUS,
            center + BALL_RADIUS + ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center + BALL_RADIUS,
            center + BALL_RADIUS + ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center + BALL_RADIUS + ARROW_SIZE,
            center + BALL_RADIUS,
            center + BALL_RADIUS + ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE
        );
    }

    if (ball.x_velocity < 0 && ball.y_velocity > 0) {
        SDL_RenderDrawLine(
            renderer,
            center - BALL_RADIUS,
            center + BALL_RADIUS,
            center - BALL_RADIUS - ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center - BALL_RADIUS,
            center + BALL_RADIUS + ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center - BALL_RADIUS - ARROW_SIZE,
            center + BALL_RADIUS,
            center - BALL_RADIUS - ARROW_SIZE,
            center + BALL_RADIUS + ARROW_SIZE
        );
    }

    if (ball.x_velocity < 0 && ball.y_velocity < 0) {
        SDL_RenderDrawLine(
            renderer,
            center - BALL_RADIUS,
            center - BALL_RADIUS,
            center - BALL_RADIUS - ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center - BALL_RADIUS,
            center - BALL_RADIUS - ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE
        );
        SDL_RenderDrawLine(
            renderer,
            center - BALL_RADIUS - ARROW_SIZE,
            center - BALL_RADIUS,
            center - BALL_RADIUS - ARROW_SIZE,
            center - BALL_RADIUS - ARROW_SIZE
        );
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

#define RANDOM_BALL_SPEED (rand() % 3) == 1 ? BALL_SPEED : -BALL_SPEED
void randomize_ball_speed(struct Ball* ball) {
    ball->x_velocity = RANDOM_BALL_SPEED;
    ball->y_velocity = RANDOM_BALL_SPEED;
}
#undef RANDOM_BALL_SPEED
