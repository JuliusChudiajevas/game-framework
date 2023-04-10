#pragma once
#include <iostream>
#include <string>

#include "Game.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
class TextRenderer {
 public:
  static void drawText(std::string text, int x, int y, int r, int g, int b, int fontSize) {
    SDL_Surface* tempSurface;
    SDL_Texture* tempTexture;
    TTF_Font* font = TTF_OpenFont("assets/fonts/Arial.ttf", fontSize);

    if (font == nullptr) {
      std::cout << "no FONT" << TTF_GetError() << std::endl;
      return;
    }

    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;

    SDL_Rect rect;
    tempSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    tempTexture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    rect.x = x;
    rect.y = y;
    rect.w = tempSurface->w;
    rect.h = tempSurface->h;
    SDL_FreeSurface(tempSurface);
    SDL_RenderCopy(Game::renderer, tempTexture, nullptr, &rect);
    SDL_DestroyTexture(tempTexture);
    TTF_CloseFont(font);
  }
};
