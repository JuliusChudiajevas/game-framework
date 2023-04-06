#pragma once
#include "Components.hpp"
#include "Scene.hpp"
void updatePlayerCollision(Scene& scene) {
  auto player = scene.registry.view<PlayerTagComponent, CollisionTagComponent, RectComponent>();
  for (auto [entity, rect] : player.each()) {
    auto platforms = scene.registry.view<CollisionTagComponent, RectComponent>(
        entt::exclude<PlayerTagComponent>);

    for (auto [platformEntity, platformRect] : platforms.each()) {
      //  collision
      if ((rect.right() > platformRect.left() && platformRect.right() > rect.left() &&
           rect.bottom() > platformRect.top() && platformRect.bottom() > rect.top())) {
        int overlapX = 0;
        int overlapY = 0;

        int xOverlapFromLeft = rect.right() - platformRect.left();
        int xOverlapFromRight = platformRect.right() - rect.left();
        if (std::abs(xOverlapFromLeft) < std::abs(xOverlapFromRight)) {
          overlapX = -xOverlapFromLeft;
        } else {
          overlapX = xOverlapFromRight;
        }
        rect.x += overlapX;

        int yOverlapFromTop = rect.bottom() - platformRect.top();
        int yOverlapFromBottom = platformRect.bottom() - rect.top();
        if (std::abs(yOverlapFromTop) < std::abs(yOverlapFromBottom)) {
          overlapY = yOverlapFromTop;
        } else {
          overlapY = yOverlapFromBottom;
        }

        rect.y += overlapY;
      }
    }
  }
}

void updatePlayerCollisionX(Scene& scene) {
  auto player = scene.registry.view<PlayerTagComponent, CollisionTagComponent, RectComponent>();
  for (auto [entity, rect] : player.each()) {
    auto platforms = scene.registry.view<CollisionTagComponent, RectComponent>(
        entt::exclude<PlayerTagComponent>);

    for (auto [platformEntity, platformRect] : platforms.each()) {
      //  collision
      if ((rect.right() > platformRect.left() && platformRect.right() > rect.left() &&
           rect.bottom() > platformRect.top() && platformRect.bottom() > rect.top())) {
        int overlapX = 0;
        int xOverlapFromLeft = rect.right() - platformRect.left();
        int xOverlapFromRight = platformRect.right() - rect.left();
        if (std::abs(xOverlapFromLeft) < std::abs(xOverlapFromRight)) {
          overlapX = -xOverlapFromLeft;
        } else {
          overlapX = xOverlapFromRight;
        }
        rect.x += overlapX;
      }
    }
  }
}

void updatePlayerCollisionY(Scene& scene) {
  auto player = scene.registry.view<PlayerTagComponent, CollisionTagComponent, RectComponent>();
  for (auto [entity, rect] : player.each()) {
    auto platforms = scene.registry.view<CollisionTagComponent, RectComponent>(
        entt::exclude<PlayerTagComponent>);

    for (auto [platformEntity, platformRect] : platforms.each()) {
      //  collision
      if ((rect.right() > platformRect.left() && platformRect.right() > rect.left() &&
           rect.bottom() > platformRect.top() && platformRect.bottom() > rect.top())) {
        int overlapY = 0;
        int yOverlapFromTop = rect.bottom() - platformRect.top();
        int yOverlapFromBottom = platformRect.bottom() - rect.top();
        if (std::abs(yOverlapFromTop) < std::abs(yOverlapFromBottom)) {
          overlapY = -yOverlapFromTop;
        } else {
          overlapY = yOverlapFromBottom;
        }
        rect.y += overlapY;
      }
    }
  }
}