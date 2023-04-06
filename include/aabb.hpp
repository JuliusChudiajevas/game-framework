#pragma once
#include "Components.hpp"
#include "Game.hpp"

inline bool aabb(RectComponent& rect1, RectComponent& rect2) {
  if ((rect1.right() > rect2.left() && rect2.right() > rect1.left() &&
       rect1.bottom() > rect2.top() && rect2.bottom() > rect1.top())) {
    return true;
  }
  return false;
}
inline void clearScreen() {
  SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255);
  SDL_RenderClear(Game::renderer);
}