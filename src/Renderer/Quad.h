#pragma once
#include "Common.h"

class QuadRenderer
{
public:
  static void Quad(const glm::vec2& v1,
                   const glm::vec2& v2,
                   const glm::vec2& v3,
                   const glm::vec2& v4);
  static void Rectangle(const glm::vec2& topLeft, const glm::vec2& bottomRight);

protected:
  static void Init(glm::mat4* view, glm::mat4* projection);
  static void Begin();
  static void End();
};
