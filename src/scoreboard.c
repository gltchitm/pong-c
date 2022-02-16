#include "scoreboard.h"

static void draw_score(int current_score, int x, int y, SDL_Renderer* renderer) {
    TTF_Font *font = TTF_OpenFont("./vendor/font/font.ttf", 48);
    SDL_Color color = {255, 255, 255, 255};

    char score[7];
    sprintf(score, "%d", current_score);

    SDL_Surface* surface_message = TTF_RenderText_Solid(font, score, color);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surface_message);
    SDL_Rect message_rect = {
        x - surface_message->w / 2,
        y,
        surface_message->w,
        surface_message->h
    };

    SDL_RenderCopy(renderer, message, NULL, &message_rect);
    SDL_FreeSurface(surface_message);
    SDL_DestroyTexture(message);

    TTF_CloseFont(font);
}

void draw_scoreboard(int left_score, int right_score, int width, SDL_Renderer* renderer) {
    draw_score(left_score, width / 4, SCOREBOARD_PADDING_TOP, renderer);
    draw_score(right_score, (width / 4) * 3, SCOREBOARD_PADDING_TOP, renderer);
}
