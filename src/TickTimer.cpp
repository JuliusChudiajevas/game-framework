#include "TickTimer.hpp"

#include "SDL2/SDL.h"

TickTimer::TickTimer(Uint64 timeOutMs) : startTime(SDL_GetTicks64()), timeOutMs(timeOutMs) {
}

bool TickTimer::hasElapsed() {
  if (SDL_GetTicks64() > startTime + timeOutMs) {
    startTime = SDL_GetTicks64();
    return true;
  }
  return false;
}