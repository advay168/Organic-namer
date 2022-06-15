#pragma once
#include "Common.h"

class CircleRenderer
{
public:
  static void Circle(const glm::vec2& center, float radius);

  static void Circle(const glm::vec2& center,
                     float radius,
                     const glm::vec3& color);

  static void BorderCircle(const glm::vec2& center,
                           float innerRadius,
                           const glm::vec3& innerColor,
                           float borderThickness,
                           const glm::vec3& borderColor);

protected:
  static void Init();
  static void Begin();
  static void End();
};
