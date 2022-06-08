#pragma once

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "Shader.h"

struct CircleData
{
  glm::vec2 center;
  float radius;

  bool operator==(const CircleData& other)
  {
    return center == other.center && radius == other.radius;
  }
};

class Circle
{
public:
  Circle();
  Circle(const glm::vec2& center, float radius);
  ~Circle();

  static void init(GLFWwindow* window);
  static void draw();

  CircleData m_data;
};