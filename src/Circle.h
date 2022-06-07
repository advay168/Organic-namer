#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

#include <vector>

struct CircleData
{
  glm::vec2 m_center;
  float m_radius;
};

class Circle
{
public:
  Circle();
  Circle(const glm::vec2& center, float radius);
  CircleData m_data;
};