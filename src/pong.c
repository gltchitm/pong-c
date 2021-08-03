#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "scoreboard.h"
#include "welcome.h"
#include "paddles.h"
#include "ball.h"

const int WINDOW_WIDTH = 750;
const int WINDOW_HEIGHT = 750;
const int CAP_PAD = 15;
const int SECOND = 1000;
const int SPEED = 300;
const int BALL_SPEED = 165;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    
    TTF_Init();

    srand(time(NULL));

    const int paddle_bottom = WINDOW_HEIGHT - PADDLE_HEIGHT - CAP_PAD;

    struct Paddles paddles;
    paddles.left_paddle = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddles.right_paddle = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddles.left_paddle_proposed = 0;
    paddles.right_paddle_proposed = 0;

    struct Ball ball;
    ball.x = WINDOW_WIDTH / 2 - BALL_RADIUS / 2;
    ball.y = WINDOW_HEIGHT / 2 - BALL_RADIUS / 2;

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);

    int close = 0;
    int left_score = 0;
    int right_score = 0;
    int game_active = 0;

    ball.x_speed = (rand() % 3) == 1 ? BALL_SPEED / 30 : -BALL_SPEED / 30;
    ball.y_speed = (rand() % 3) == 1 ? BALL_SPEED / 30 : -BALL_SPEED / 30;

    while (!close) {
        SDL_Event event;
        if (game_active) {
            paddles.left_paddle += paddles.left_paddle_proposed;
            paddles.right_paddle += paddles.right_paddle_proposed;
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    break;
                case SDL_KEYDOWN:
                    {
                        if (!game_active) {
                            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                                game_active = 1;
                            }
                            break;
                        }
                        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
                        if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                            paddles.left_paddle_proposed = -SPEED / 30;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
                            paddles.left_paddle_proposed = SPEED / 30;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                            paddles.right_paddle_proposed = -SPEED / 30;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                            paddles.right_paddle_proposed = SPEED / 30;
                        } else {
                            if (paddles.left_paddle_proposed == 0) {
                                if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                                    paddles.left_paddle_proposed = -SPEED / 30;
                                    paddles.left_paddle += paddles.left_paddle_proposed;
                                } else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
                                    paddles.left_paddle_proposed = SPEED / 30;
                                    paddles.left_paddle += paddles.left_paddle_proposed;
                                } else {
                                    if (keyboard_state[SDL_SCANCODE_W]) {
                                        paddles.left_paddle_proposed = -SPEED / 30;
                                    } else if (keyboard_state[SDL_SCANCODE_S]) {
                                        paddles.left_paddle_proposed = SPEED / 30;
                                    }
                                }
                            }
                            if (paddles.right_paddle_proposed == 0) {
                                if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                                    paddles.right_paddle_proposed = -SPEED / 30;
                                    paddles.right_paddle += paddles.right_paddle_proposed;
                                } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                                    paddles.right_paddle_proposed = SPEED / 30;
                                    paddles.right_paddle += paddles.right_paddle_proposed;
                                } else {
                                    if (keyboard_state[SDL_SCANCODE_UP]) {
                                        paddles.right_paddle_proposed = -SPEED / 30;
                                    } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
                                        paddles.right_paddle_proposed = SPEED / 30;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case SDL_KEYUP:
                    {
                        if (!game_active) {
                            break;
                        }
                        const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
                        switch (event.key.keysym.scancode) {
                            case SDL_SCANCODE_W:
                            case SDL_SCANCODE_S:
                                if (!keyboard_state[SDL_SCANCODE_W] && !keyboard_state[SDL_SCANCODE_S]) {
                                    paddles.left_paddle_proposed = 0;
                                } else if (keyboard_state[SDL_SCANCODE_W]) {
                                    paddles.left_paddle_proposed = -SPEED / 30;
                                } else if (keyboard_state[SDL_SCANCODE_S]) {
                                    paddles.left_paddle_proposed = SPEED / 30;
                                }
                                break;
                            case SDL_SCANCODE_UP:
                            case SDL_SCANCODE_DOWN:
                                if (!keyboard_state[SDL_SCANCODE_UP] && !keyboard_state[SDL_SCANCODE_DOWN]) {
                                    paddles.right_paddle_proposed = 0;
                                } else if (keyboard_state[SDL_SCANCODE_UP]) {
                                    paddles.right_paddle_proposed = -SPEED / 30;
                                } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
                                    paddles.right_paddle_proposed = SPEED / 30;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if (paddles.left_paddle > paddle_bottom) {
            paddles.left_paddle = paddle_bottom;
            paddles.left_paddle_proposed = 0;
        }
        if (paddles.left_paddle < CAP_PAD) {
            paddles.left_paddle = CAP_PAD;
            paddles.left_paddle_proposed = 0;
        }
        if (paddles.right_paddle > paddle_bottom) {
            paddles.right_paddle = paddle_bottom;
            paddles.right_paddle_proposed = 0;
        }
        if (paddles.right_paddle < CAP_PAD) {
            paddles.right_paddle = CAP_PAD;
            paddles.right_paddle_proposed = 0;
        }
        if (game_active) {
            ball.x += ball.x_speed;
            ball.y += ball.y_speed;
        }
        if (ball.y < CAP_PAD) {
            ball.y = CAP_PAD;
            ball.y_speed = -ball.y_speed;
        } else if (ball.y > WINDOW_HEIGHT - CAP_PAD) {
            ball.y = WINDOW_HEIGHT - CAP_PAD;
            ball.y_speed = -ball.y_speed;
        }
        if (
            PADDING_FROM_SIDE + PADDLE_WIDTH < ball.x + BALL_RADIUS &&
            PADDING_FROM_SIDE + PADDLE_WIDTH * 2 > ball.x &&
            paddles.left_paddle < ball.y + BALL_RADIUS &&
            paddles.left_paddle + PADDLE_HEIGHT > ball.y
        ) {
            if (ball.x_speed < 0) {
                ball.x -= ball.x_speed;
                ball.x_speed = -ball.x_speed;
            }
        }
        if (
            WINDOW_WIDTH - (PADDING_FROM_SIDE + PADDLE_WIDTH) < ball.x + BALL_RADIUS &&
            (WINDOW_WIDTH - (PADDING_FROM_SIDE + PADDLE_WIDTH)) * 2 > ball.x &&
            paddles.right_paddle < ball.y + BALL_RADIUS &&
            paddles.right_paddle + PADDLE_HEIGHT > ball.y
        ) {
            if (ball.x_speed > 0) {
                ball.x -= ball.x_speed;
                ball.x_speed = -ball.x_speed;
            }
        }
        if (
            ball.x < 0 ||
            ball.x > WINDOW_WIDTH
        ) {
            if (ball.x < 0) {
                right_score += 1;
            } else {
                left_score += 1;
            }
            ball.x = WINDOW_WIDTH / 2 - BALL_RADIUS / 2;
            ball.y = WINDOW_HEIGHT / 2 - BALL_RADIUS / 2;
            ball.x_speed = (rand() % 3) == 1 ? BALL_SPEED / 30 : -BALL_SPEED / 30;
            ball.y_speed = (rand() % 3) == 1 ? BALL_SPEED / 30 : -BALL_SPEED / 30;
        }
        SDL_RenderClear(renderer);
        if (game_active) {
            draw_ball(&ball, renderer);
            draw_scoreboard(left_score, right_score, WINDOW_WIDTH, renderer);
            draw_paddles(&paddles, WINDOW_WIDTH, renderer);
        } else {
            draw_welcome_message(WINDOW_WIDTH, WINDOW_HEIGHT, renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(SECOND / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    return 0;
}
