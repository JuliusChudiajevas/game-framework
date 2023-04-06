#include "Game.hpp"

#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "Components.hpp"
#include "Entity.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Scene.hpp"
#include "TextRenderer.hpp"
#include "aabb.hpp"

// static class keyboard handler
void relayKeyDown(Scene& scene, SDL_Event& e) {
  auto view = scene.registry.view<KeyboardComponent>();
  for (auto [entity, keyboard] : view.each()) {
    if (keyboard.keys.count(e.key.keysym.sym)) keyboard.keys.at(e.key.keysym.sym) = true;
  }
}

void relayKeyUp(Scene& scene, SDL_Event& e) {
  auto view = scene.registry.view<KeyboardComponent>();
  for (auto [entity, keyboard] : view.each()) {
    if (keyboard.keys.count(e.key.keysym.sym)) keyboard.keys.at(e.key.keysym.sym) = false;
  }
}

void playerKeyboardLogic(Scene& scene) {
  auto view = scene.registry.view<PlayerTagComponent, KeyboardComponent, VelocityComponent,
                                  AccelerationComponent, IsInAirComponent>();
  for (auto [entity, keyboard, velocity, acceelration, isInAir] : view.each()) {
    // acceelration.yAcceleration = 0;
    if (keyboard.keys.at(SDLK_UP)) {
      if (isInAir.isInAir == false) {
        velocity.yVelocity = -0.7;
        isInAir.isInAir = true;
      }
    }

    if (keyboard.keys.at(SDLK_DOWN)) {
      acceelration.yAcceleration += 0;
    }

    acceelration.xAcceleration = 0;
    if (keyboard.keys.at(SDLK_LEFT)) {
      acceelration.xAcceleration += -0.005;
    }

    if (keyboard.keys.at(SDLK_RIGHT)) {
      acceelration.xAcceleration += 0.005;
    }
  }
}

void updateCollisionX(Scene& scene) {
  auto view1 = scene.registry.view<CollisionTagComponent, RectComponent, VelocityComponent>();
  for (auto [entity1, rect1, velocity] : view1.each()) {
    auto view2 = scene.registry.view<CollisionTagComponent, RectComponent>();

    for (auto [entity2, rect2] : view2.each()) {
      // dont collide to yourself
      if (entity1 == entity2) {
        continue;
      }

      // if no collision
      if (!aabb(rect1, rect2)) {
        continue;
      }

      float overlapX = 0;

      float xOverlapFromLeft = rect1.right() - rect2.left();
      float xOverlapFromRight = rect2.right() - rect1.left();

      if (std::abs(xOverlapFromLeft) < std::abs(xOverlapFromRight)) {
        overlapX = -xOverlapFromLeft;
      } else {
        overlapX = xOverlapFromRight;
      }
      rect1.x += overlapX;
      rect1.x = std::round(rect1.x);
    }
  }
}

void updateCollisionY(Scene& scene) {
  auto view1 = scene.registry.view<CollisionTagComponent, RectComponent, VelocityComponent,
                                   AccelerationComponent, IsInAirComponent>();
  for (auto [entity1, rect1, velocity1, acceleration1, isInAir1] : view1.each()) {
    auto view2 = scene.registry.view<CollisionTagComponent, RectComponent>();

    for (auto [entity2, rect2] : view2.each()) {
      // dont collide to yourself
      if (entity1 == entity2) {
        continue;
      }

      // if no collision
      if (!aabb(rect1, rect2)) {
        isInAir1.isInAir = true;
        continue;
      }

      float overlapY = 0;

      float yOverlapFromTop = rect1.bottom() - rect2.top();
      float yOverlapFromBottom = rect2.bottom() - rect1.top();

      if (std::abs(yOverlapFromTop) < std::abs(yOverlapFromBottom)) {
        overlapY = -yOverlapFromTop;
        isInAir1.isInAir = false;
        velocity1.yVelocity = 0;
        acceleration1.yAcceleration = 0;
      } else {
        overlapY = yOverlapFromBottom;
        velocity1.yVelocity = -velocity1.yVelocity;
      }

      rect1.y += overlapY;
      rect1.y = std::round(rect1.y);
      return;
    }
  }
}

void restart(Scene& scene) {
  auto projectiles = scene.registry.view<ProjectileTagComponent>();

  for (auto [entityId] : projectiles.each()) {
    scene.registry.destroy(entityId);
  }

  auto player = scene.registry.view<PlayerTagComponent, RectComponent>();

  for (auto [entityId, rect] : player.each()) {
    rect.x = 200;
    rect.y = 200;
  }
  Game::bestTime = Game::elapsedTime;
  Game::elapsedTime = 0;
}

void projectileLogic(Scene& scene) {
  auto player = scene.registry.view<PlayerTagComponent, RectComponent>();
  auto projectiles =
      scene.registry.view<ProjectileTagComponent, RectComponent, AccelerationComponent>();
  for (auto [entity1, rect1, acceleration1] : projectiles.each()) {
    // float random = static_cast<float>((std::rand() % 10)) / 10;
    int random = std::rand() % 100000;
    if (random % 2 == 0) {
      random = -random;
    }

    // player
    for (auto [pentity, prect] : player.each()) {
      if (aabb(rect1, prect)) {
        restart(scene);
      }
      acceleration1.xAcceleration = -((rect1.x - prect.x) / (200000));
      acceleration1.yAcceleration = -((rect1.y - prect.y) / (200000 - random));
    }

    for (auto [entity2, rect2, acceelration2] : projectiles.each()) {
      // dont collide to yourself
      if (entity1 == entity2) {
        continue;
      }

      // if no collision
      if (!aabb(rect1, rect2)) {
        continue;
      }

      acceelration2.yAcceleration = 0;
      acceelration2.xAcceleration = 0;
    }
  }
}

void updateGravity(Scene& scene) {
  auto view = scene.registry.view<AccelerationComponent, GravityComponent>();
  for (auto [entity, acceleration, gravity] : view.each()) {
    acceleration.yAcceleration += gravity.gravity * Game::updateDeltaTime;
  }
}

void updateVelocity(Scene& scene) {
  auto view = scene.registry.view<AccelerationComponent, VelocityComponent>();
  for (auto [entity, acceleration, velocity] : view.each()) {
    if (acceleration.xAcceleration == 0) {
      velocity.xVelocity = 0;
    }

    if (acceleration.yAcceleration == 0) {
      velocity.yVelocity = 0;
    }

    velocity.xVelocity += acceleration.xAcceleration * Game::updateDeltaTime;
    if (velocity.xVelocity > velocity.xMaxVelocity) velocity.xVelocity = velocity.xMaxVelocity;
    if (velocity.xVelocity < -velocity.xMaxVelocity) velocity.xVelocity = -velocity.xMaxVelocity;

    velocity.yVelocity += acceleration.yAcceleration * Game::updateDeltaTime;
    if (velocity.yVelocity > velocity.yMaxVelocity) velocity.yVelocity = velocity.yMaxVelocity;
    if (velocity.yVelocity < -velocity.yMaxVelocity) velocity.yVelocity = -velocity.yMaxVelocity;
  }
}

void updateRect(Scene& scene) {
  auto view = scene.registry.view<RectComponent, VelocityComponent>();
  for (auto [entity, rect, velocity] : view.each()) {
    rect.x += velocity.xVelocity * Game::updateDeltaTime;
    updateCollisionX(scene);
    rect.y += velocity.yVelocity * Game::updateDeltaTime;
    updateCollisionY(scene);
  }
}

void updateSprite(Scene& scene) {
  auto view = scene.registry.view<RectComponent, SpriteComponent>();
  for (auto [entity, rect, texture] : view.each()) {
    texture.dstRect.x = rect.x;
    texture.dstRect.y = rect.y;
  }
}

//  private:
//   bool isRunning;
//   SDL_Window* window;

Game::Game() {
}

Game::~Game() {
}

SDL_Renderer* Game::renderer = nullptr;
Uint64 Game::updateDeltaTime = 1;
Uint64 Game::updateEndTime = 1;
Uint64 Game::updateStartTime = 1;
float Game::elapsedTime = 0;
float Game::bestTime = 0;

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen) {
  int flags = 0;

  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    isRunning = false;
    return;
  }

  if (TTF_Init()) {
    std::cout << "TTF not initialized " << TTF_GetError() << std::endl;
    return;
  }

  std::cout << "initializing" << std::endl;

  window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

  if (window) {
    std::cout << "window created" << std::endl;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    std::cout << "renderer created" << std::endl;
  }

  isRunning = true;
}

void Game::handleEvents(Scene& scene) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
        relayKeyDown(scene, event);
        break;
      case SDL_KEYUP:
        relayKeyUp(scene, event);
        break;
      case SDL_QUIT:
        isRunning = false;
        break;
      default:
        break;
    }
  }
}

void Game::update(Scene& scene) {
  updateEndTime = SDL_GetTicks64();
  updateDeltaTime = updateEndTime - updateStartTime;
  if (updateDeltaTime > 20) updateDeltaTime = 0;
  playerKeyboardLogic(scene);
  projectileLogic(scene);
  updateGravity(scene);
  updateVelocity(scene);
  updateRect(scene);
  updateStartTime = updateEndTime;
}

void Game::render(Scene& scene) {
  clearScreen();
  updateSprite(scene);
  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  auto view = scene.registry.view<RectComponent, RectColorComponent>();
  for (auto [entity, rect, rectColor] : view.each()) {
    SDL_Rect temp;
    temp.x = rect.x;
    temp.y = rect.y;
    temp.w = rect.width;
    temp.h = rect.height;
    SDL_SetRenderDrawColor(renderer, rectColor.color.r, rectColor.color.g, rectColor.color.b,
                           rectColor.color.a);

    SDL_RenderDrawRect(renderer, &temp);
  }
  Game::elapsedTime += Game::updateDeltaTime / 1000.0;
  TextRenderer::drawText("Seconds passed: " + std::to_string(Game::elapsedTime), 350, 70, 222, 255,
                         255, 18);
  TextRenderer::drawText("Best time: " + std::to_string(Game::bestTime), 350, 850, 222, 200, 255,
                         18);
  SDL_RenderPresent(renderer);
}

void Game::cleanup() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

bool Game::running() {
  return isRunning;
}