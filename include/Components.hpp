#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "SDL2/SDL_image.h"
#include "TextureManager.hpp"

struct TagComponent {
  std::string tag;
};

struct RectComponent {
  float x, y, width, height;
  RectComponent() : x(0), y(0), width(10), height(10) {
  }
  RectComponent(float x, float y, float width, float height)
      : x(x), y(y), width(width), height(height) {
  }
  float left() {
    return x;
  }
  float right() {
    return x + width;
  }
  float top() {
    return y;
  }
  float bottom() {
    return y + height;
  }
};

struct PlayerTagComponent {};

struct KeyboardComponent {
  std::map<SDL_Keycode, bool> keys;
  KeyboardComponent(std::vector<SDL_Keycode>&& assignedKeys) {
    for (auto key : assignedKeys) {
      keys.emplace(key, false);
    }
  }
};

static int SCALE_FACTOR = 1;

struct SpriteComponent {
  SDL_Texture* texture = nullptr;
  SDL_Rect srcRect, dstRect;

  SpriteComponent() {
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = 32 * SCALE_FACTOR;
    dstRect.h = 32 * SCALE_FACTOR;
  }

  SpriteComponent(float width, float height) {
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;

    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = width * SCALE_FACTOR;
    dstRect.h = height * SCALE_FACTOR;
  }
};

struct CollisionTagComponent {};

struct ProjectileTagComponent {};

struct RectColorComponent {
  SDL_Color color;

  RectColorComponent() {
    color.r = 255;
    color.g = 0;
    color.b = 0;
    color.a = 255;
  }

  RectColorComponent(int r, int g, int b) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;
  }

  RectColorComponent(int r, int g, int b, int a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
  }
};

struct GravityComponent {
  float gravity = 0.00002;
};
struct AccelerationComponent {
  float xAcceleration = 0.0;
  float yAcceleration = 0.0;
};

struct VelocityComponent {
  VelocityComponent(float xMaxVelocity, float yMaxVelocity)
      : xMaxVelocity(xMaxVelocity), yMaxVelocity(yMaxVelocity) {
  }
  VelocityComponent(float xVelocity, float yVelocity, float xMaxVelocity, float yMaxVelocity)
      : xVelocity(xVelocity),
        yVelocity(yVelocity),
        xMaxVelocity(xMaxVelocity),
        yMaxVelocity(yMaxVelocity) {
  }
  float xVelocity = 0.0;
  float yVelocity = 0.0;
  float xMaxVelocity = 1;
  float yMaxVelocity = 1;
};

struct IsInAirComponent {
  bool isInAir = true;
};

struct ButtonComponent {
  bool isHovered = false;
  std::string text;
};
