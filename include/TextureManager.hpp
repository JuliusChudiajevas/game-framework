#pragma once

#include <SDL2/SDL_image.h>

class TextureManager {
 public:
  static SDL_Texture* loadTexture(const char* fileName);
  static void Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect dstRect);
};