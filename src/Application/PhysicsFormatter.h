#pragma once
#include "Common.h"

#include "Scene.h"

class PhysicsFormatter
{
public:
  PhysicsFormatter(Scene& scene);

  void exertForce();
  void applyForce();

  void draw();

private:
  void optimiseForce();

private:
  Scene& scene;
  static constexpr float bondLength = 340.0f;
};
