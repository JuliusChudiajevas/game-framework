#pragma once

#include "SDL2/SDL.h"
#include "Scene.hpp"
#include "entt.hpp"

class Entity {
 private:
  Scene* scene = nullptr;
  entt::entity entityHandle = entt::entity(0);

 public:
  Entity(entt::entity entityHandle, Scene* ptrScene) : entityHandle(entityHandle), scene(ptrScene) {
  }

  template <typename T>
  void addTag() {
    scene->registry.emplace<T>(entityHandle);
  }

  template <typename T, typename... Args>
  T& addComponent(Args&&... args) {
    return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
  }

  template <typename T>
  T& getComponent() {
    return scene->registry.get<T>(entityHandle);
  }

  template <typename T>
  void removeComponent() {
    return scene->registry.remove<T>(entityHandle);
  }

  template <typename T>
  bool hasComponent() {
    return scene->registry.all_of<T>(entityHandle);
  }
};