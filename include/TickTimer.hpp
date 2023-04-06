#pragma once
#include "SDL2/SDL.h"

class TickTimer {
 private:
  Uint64 startTime;
  Uint64 timeOutMs;

 public:
  TickTimer(Uint64 timeOutMs);

  bool hasElapsed();
};