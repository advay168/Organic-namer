#pragma once
#include "Common.h"

class TextCircleRenderer
{
public:
  static void TextCircle(const glm::vec2& center,
                         float radius,
                         const glm::vec3& circleColor,
                         const std::string& text,
                         float scale,
                         const glm::vec3& textColor);
};
