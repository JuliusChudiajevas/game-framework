#include <iostream>

#include "Components.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "SDL2/SDL.h"
#include "Scene.hpp"
#include "TextRenderer.hpp"
#include "TickTimer.hpp"
#include "aabb.hpp"
#include "entt.hpp"

// extern bool aabb(RectComponent&, RectComponent&);
// extern void clearScreen();

void loadTexture(Scene& scene) {
  auto view = scene.registry.view<PlayerTagComponent, SpriteComponent>();
  for (auto [entity, textureComponent] : view.each()) {
    textureComponent.texture = TextureManager::loadTexture("assets/Sprite1.png");
  }
}

void spawnProjectile(Scene& scene) {
  Entity projectile = scene.createEntity();
  auto projectileRect = projectile.addComponent<RectComponent>(RectComponent{10, 10, 10, 10});
  projectile.addTag<ProjectileTagComponent>();
  projectile.addComponent<RectColorComponent>(RectColorComponent{255, 100, 100});
  projectile.addComponent<VelocityComponent>(VelocityComponent{0.07, 0.07});
  projectile.addComponent<AccelerationComponent>();
  projectile.addComponent<SpriteComponent>(
      SpriteComponent{projectileRect.width, projectileRect.height});

  auto view = scene.registry.view<ProjectileTagComponent, VelocityComponent>();
  for (auto [entity, velocity] : view.each()) {
    velocity.xMaxVelocity += 0.006;
    velocity.yMaxVelocity += 0.006;
  }
}

void spawnGround(Scene& scene, float x, float y, float width, float height) {
  Entity ground = scene.createEntity();
  ground.addTag<CollisionTagComponent>();
  auto groundRect = ground.addComponent<RectComponent>(x, y, width, height);
  ground.addComponent<RectColorComponent>(RectColorComponent{110, 200, 60});
  ground.addComponent<SpriteComponent>(SpriteComponent{groundRect.width, groundRect.height});
}

void addButton(Scene& menuScene, float x, float y, float width, float height, std::string text) {
  Entity button = menuScene.createEntity();
  button.addComponent<ButtonComponent>(ButtonComponent{false, text});
  auto buttonRect = button.addComponent<RectComponent>(RectComponent{x, y, width, height});
  button.addComponent<RectColorComponent>(RectColorComponent{255, 255, 255});
}

void showMenu(Scene& menu) {
  RectComponent mousePos;
  SDL_Event event;
  while (true) {
    auto buttons = menu.registry.view<ButtonComponent, RectComponent, RectColorComponent>();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_MOUSEMOTION:
          mousePos.x = event.motion.x;
          mousePos.y = event.motion.y;
          break;
        case SDL_MOUSEBUTTONDOWN:
          for (auto [entityId, button, rect, rectColor] : buttons.each()) {
            if (button.isHovered) {
              return;
            }
          }
          break;
        default:
          break;
      }
    }

    // draw buttons
    // hover logic
    clearScreen();
    for (auto [entityId, button, rect, rectColor] : buttons.each()) {
      if (aabb(mousePos, rect)) {
        rectColor.color.a = 255;
        rectColor.color.b = 0;
        button.isHovered = true;
      } else {
        rectColor.color.a = 100;
        rectColor.color.b = 233;
        button.isHovered = false;
      }

      SDL_Rect temp;
      temp.x = rect.x;
      temp.y = rect.y;
      temp.w = rect.width;
      temp.h = rect.height;
      SDL_SetRenderDrawColor(Game::renderer, rectColor.color.r, rectColor.color.g,
                             rectColor.color.b, rectColor.color.a);

      TextRenderer::drawText(button.text, rect.x, rect.y, rectColor.color.r, rectColor.color.g,
                             rectColor.color.b, 15);

      SDL_RenderDrawRect(Game::renderer, &temp);
      SDL_RenderPresent(Game::renderer);
    }
  }
}

int main(int argv, char** args) {
  std::unique_ptr<Game> game = std::make_unique<Game>();

  game->init("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, false);

  TickTimer updateTimer(1000 / 250);
  TickTimer renderTimer(1000 / 144);
  TickTimer spawnProjectileTimer(4000);

  Scene scene;
  Scene startMenu;
  addButton(startMenu, 300, 400, 200, 100, "START");

  float SCALE = 1;
  Entity player = scene.createEntity();
  player.addTag<PlayerTagComponent>();
  player.addTag<CollisionTagComponent>();
  player.addComponent<GravityComponent>();
  player.addComponent<SpriteComponent>();
  player.addComponent<KeyboardComponent>(
      std::vector<SDL_Keycode>{SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT});
  player.addComponent<RectComponent>(200, 200, 20, 20);
  player.addComponent<RectColorComponent>(RectColorComponent{255, 255, 255});
  player.addComponent<VelocityComponent>(VelocityComponent(0.5, 1));
  player.addComponent<AccelerationComponent>();
  player.addComponent<IsInAirComponent>();

  auto groundTexture = TextureManager::loadTexture("assets/testCollision.png");

  // top
  spawnGround(scene, 50, 50, 800, 60);
  // bottom
  spawnGround(scene, 50, 790, 800, 60);
  // left
  spawnGround(scene, 50, 50, 60, 800);
  // right
  spawnGround(scene, 790, 50, 60, 800);
  // platform middle
  spawnGround(scene, 340, 700, 200, 20);
  // platform middle high
  spawnGround(scene, 400, 330, 100, 20);
  // platform left
  spawnGround(scene, 200, 630, 100, 20);
  // platform left a bit higher
  spawnGround(scene, 170, 530, 50, 20);
  // platform left a bit higher a bit higher
  spawnGround(scene, 270, 430, 50, 20);
  // platform right
  spawnGround(scene, 600, 630, 100, 20);
  // platform right a bit higher
  spawnGround(scene, 670, 530, 70, 20);
  // platform right a bit left a bit higher a bit higher
  spawnGround(scene, 600, 430, 50, 20);

  loadTexture(scene);

  showMenu(startMenu);

  while (game->running()) {
    game->handleEvents(scene);

    if (spawnProjectileTimer.hasElapsed()) {
      spawnProjectile(scene);
    }

    if (updateTimer.hasElapsed()) {
      game->update(scene);
    }

    if (renderTimer.hasElapsed()) {
      game->render(scene);
    }
  }

  game->cleanup();
  return 0;
}