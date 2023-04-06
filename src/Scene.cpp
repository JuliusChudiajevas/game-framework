#include "Scene.hpp"

#include "Components.hpp"
#include "Entity.hpp"
#include "entt.hpp"

Scene::Scene() {
}

Scene::~Scene() {
}

Entity Scene::createEntity() {
  Entity entity(registry.create(), this);
  return entity;
}

Entity Scene::createEntity(std::string tag) {
  Entity entity(registry.create(), this);
  entity.addComponent<TagComponent>("ok");
  return entity;
}