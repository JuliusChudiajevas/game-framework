#pragma once
#include <iostream>
#include <vector>

#include "Entity.hpp"
#include "SDL2/SDL.h"

class Game {
 private:
  bool isRunning;
  SDL_Window* window;

 public:
  Game();
  ~Game();
  static Uint64 updateStartTime;
  static Uint64 updateEndTime;
  static Uint64 updateDeltaTime;
  static float elapsedTime;
  static float bestTime;

  static SDL_Renderer* renderer;

  void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

  void handleEvents(Scene& scene);

  void update(Scene& scene);

  void render(Scene& scene);

  void cleanup();

  bool running();
};