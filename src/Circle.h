#pragma once

#include "Common.h"

#include "Shader.h"

struct CircleData
{
  glm::vec2 center;
  float radius;
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