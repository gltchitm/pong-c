#include "welcome.h"

static void draw_centered_message(int width, int font_size, int y, char* message, SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("./vendor/font/font.ttf", font_size);
    SDL_Color color = {195, 195, 195, 0};

    SDL_Surface* pong_message = TTF_RenderText_Solid(font, message, color);
    SDL_Texture* pong_surface = SDL_CreateTextureFromSurface(renderer, pong_message);

    SDL_Rect pong_rect;
    pong_rect.x = width / 2 - pong_message->w / 2;
    pong_rect.y = y;
    pong_rect.w = pong_message->w;
    pong_rect.h = pong_message->h;

    SDL_RenderCopy(renderer, pong_surface, NULL, &pong_rect);
    SDL_FreeSurface(pong_message);
    SDL_DestroyTexture(pong_surface);

    TTF_CloseFont(font);
}

void draw_welcome_message(int width, int height, SDL_Renderer* renderer) {
    char* pong_name = "Pong C";
    char* press_space_to_start = "Press SPACE To Start!";
    draw_centered_message(width, 64, height / 3, pong_name, renderer);
    draw_centered_message(width, 24, height / 2, press_space_to_start, renderer);
}
