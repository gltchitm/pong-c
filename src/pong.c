#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include "constants.h"
#include "scoreboard.c"
#include "welcome.c"
#include "pressed_keys.h"
#include "paddles.c"
#include "ball.c"

int clamp(int value, int min, int max) {
    return value < min ? min : value > max ? max : value;
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pong C",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    TTF_Init();

    srand(time(NULL));

    struct Paddles paddles = {
        WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2,
        0,
        WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2,
        0
    };
    struct Ball ball = {
        WINDOW_WIDTH / 2 - BALL_RADIUS / 2,
        WINDOW_HEIGHT / 2 - BALL_RADIUS / 2,
        0,
        0
    };

    struct PressedKeys pressed_keys = {
        KEY_PRESS_STATE_INACTIVE,
        KEY_PRESS_STATE_INACTIVE,
        KEY_PRESS_STATE_INACTIVE,
        KEY_PRESS_STATE_INACTIVE
    };

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    int left_score = 0;
    int right_score = 0;
    int playing = false;

    unsigned long long tick = 0;

    randomize_ball_speed(&ball);

    SDL_Event event;
    while (true) {
        struct timespec tick_start, tick_end;

        clock_gettime(CLOCK_MONOTONIC_RAW, &tick_start);

        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    goto exit;
                case SDL_KEYDOWN:
                    #define KEYDOWN_HANDLER(key, inverse) \
                        if (pressed_keys.inverse == KEY_PRESS_STATE_ACTIVE) { \
                            pressed_keys.inverse = KEY_PRESS_STATE_BACKGROUND; \
                        } \
                        pressed_keys.key = KEY_PRESS_STATE_ACTIVE;

                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_SPACE:
                            playing = true;
                            break;
                        case SDL_SCANCODE_W:
                            KEYDOWN_HANDLER(w, s)
                            break;
                        case SDL_SCANCODE_S:
                            KEYDOWN_HANDLER(s, w)
                            break;
                        case SDL_SCANCODE_UP:
                            KEYDOWN_HANDLER(up, down)
                            break;
                        case SDL_SCANCODE_DOWN:
                            KEYDOWN_HANDLER(down, up)
                            break;
                        default:
                            break;
                    }

                    #undef KEYDOWN_HANDLER

                    break;
                case SDL_KEYUP:
                    if (!playing) {
                        break;
                    }

                    #define KEYUP_HANDLER(key, inverse) \
                        pressed_keys.key = KEY_PRESS_STATE_INACTIVE; \
                        if (pressed_keys.inverse == KEY_PRESS_STATE_BACKGROUND) { \
                            pressed_keys.inverse = KEY_PRESS_STATE_ACTIVE; \
                        }

                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            KEYUP_HANDLER(w, s)
                            break;
                        case SDL_SCANCODE_S:
                            KEYUP_HANDLER(s, w)
                            break;
                        case SDL_SCANCODE_UP:
                            KEYUP_HANDLER(up, down)
                            break;
                        case SDL_SCANCODE_DOWN:
                            KEYUP_HANDLER(down, up)
                            break;
                        default:
                            break;
                    }

                    #undef KEYUP_HANDLER

                    break;
                default:
                    break;
            }
        }

        if (pressed_keys.w == KEY_PRESS_STATE_ACTIVE) {
            paddles.left_paddle -= PADDLE_SPEED;
        }
        if (pressed_keys.s == KEY_PRESS_STATE_ACTIVE) {
            paddles.left_paddle += PADDLE_SPEED;
        }

        if (pressed_keys.up == KEY_PRESS_STATE_ACTIVE) {
            paddles.right_paddle -= PADDLE_SPEED;
        }
        if (pressed_keys.down == KEY_PRESS_STATE_ACTIVE) {
            paddles.right_paddle += PADDLE_SPEED;
        }

        #define CLAMP_PADDLE(left_or_right) \
            paddles.left_or_right##_paddle = clamp( \
                paddles.left_or_right##_paddle, \
                PADDING, \
                WINDOW_HEIGHT - PADDLE_HEIGHT - PADDING \
            )

        CLAMP_PADDLE(left);
        CLAMP_PADDLE(right);

        #undef CLAMP_PADDLE

        if (playing && (++tick >= TICKS_UNTIL_START)) {
            ball.x += ball.x_velocity;
            ball.y += ball.y_velocity;
        }

        if (
            ball.x < PADDING + BALL_RADIUS ||
            ball.x > WINDOW_WIDTH - PADDING - BALL_RADIUS
        ) {
            if (ball.x < PADDING + BALL_RADIUS) {
                right_score = clamp(right_score + 1, 0, MAX_SCORE);
            } else {
                left_score = clamp(left_score + 1, 0, MAX_SCORE);
            }

            ball.x = WINDOW_WIDTH / 2 - BALL_RADIUS / 2;
            ball.y = WINDOW_HEIGHT / 2 - BALL_RADIUS / 2;
            tick = 0;

            randomize_ball_speed(&ball);
        } else {
            if (ball.y < PADDING + BALL_RADIUS) {
                ball.y = PADDING + BALL_RADIUS;
                ball.y_velocity *= -1;
            } else if (ball.y > WINDOW_HEIGHT - PADDING - BALL_RADIUS) {
                ball.y = WINDOW_HEIGHT - PADDING - BALL_RADIUS;
                ball.y_velocity *= -1;
            }

            if (
                (ball.x < PADDING * 2 + PADDLE_WIDTH + BALL_RADIUS) &&
                (ball.y + BALL_RADIUS > paddles.left_paddle) &&
                (ball.y - BALL_RADIUS < paddles.left_paddle + PADDLE_HEIGHT)
            ) {
                ball.x = PADDING * 2 + PADDLE_WIDTH + BALL_RADIUS;
                ball.x_velocity *= -1;
            }

            if (
                (ball.x > WINDOW_WIDTH - PADDING * 2 - PADDLE_WIDTH - BALL_RADIUS) &&
                (ball.y + BALL_RADIUS > paddles.right_paddle) &&
                (ball.y - BALL_RADIUS < paddles.right_paddle + PADDLE_HEIGHT)
            ) {
                ball.x = WINDOW_WIDTH - PADDING * 2 - PADDLE_WIDTH - BALL_RADIUS;
                ball.x_velocity *= -1;
            }
        }

        if (playing) {
            draw_ball(ball, renderer);
            draw_scoreboard(left_score, right_score, WINDOW_WIDTH, renderer);
            draw_paddles(paddles, WINDOW_WIDTH, renderer);
        } else {
            draw_welcome_message(WINDOW_WIDTH, WINDOW_HEIGHT, renderer);
        }

        int center = WINDOW_HEIGHT / 2 - BALL_RADIUS / 2;

        #define TICK_IN_RANGE(from, to) \
            ( \
                tick >= from && \
                tick <= to \
            )

        int ticks_until_start_sixth = TICKS_UNTIL_START / 6;

        if (
            TICK_IN_RANGE(ticks_until_start_sixth, ticks_until_start_sixth * 2) ||
            TICK_IN_RANGE(ticks_until_start_sixth * 3, ticks_until_start_sixth * 4) ||
            TICK_IN_RANGE(ticks_until_start_sixth * 5, ticks_until_start_sixth * 6)
        ) {
            draw_arrow(ball, renderer);
        }

        #undef TICK_IN_RANGE

        if (playing) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, center, 0, center, WINDOW_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }

        SDL_RenderPresent(renderer);

        clock_gettime(CLOCK_MONOTONIC_RAW, &tick_end);

        long tick_seconds = tick_end.tv_sec - tick_start.tv_sec;
        long tick_nanoseconds = tick_end.tv_nsec - tick_start.tv_nsec;
        long tick_duration = tick_seconds * 1000 + tick_nanoseconds / 1000000;

        long time_until_next_tick = TICK_DURATION - tick_duration;
        if (time_until_next_tick < 0) {
            continue;
        }

        SDL_Delay(time_until_next_tick);
    }

    exit:
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
}
