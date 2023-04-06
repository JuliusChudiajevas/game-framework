
#include "TextureManager.hpp"

#include <SDL2/SDL_image.h>

#include "Game.hpp"

SDL_Texture* TextureManager::loadTexture(const char* fileName) {
  SDL_Surface* tempSurface = IMG_Load(fileName);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
  if (texture == nullptr) {
    std::cout << "no texture: " << IMG_GetError() << std::endl;
  }
  SDL_FreeSurface(tempSurface);
  return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect dstRect) {
  SDL_RenderCopy(Game::renderer, texture, &srcRect, &dstRect);
}