#pragma once

// #include "Entity.hpp"
#include "entt.hpp"

class Entity;

class Scene {
 public:
  entt::registry registry;
  Scene();
  ~Scene();

  Entity createEntity();
  Entity createEntity(std::string tag);
};