#pragma once
#include "Common.h"

class QuadRenderer
{
public:
  static void Quad(const glm::vec2& v1,
                   const glm::vec2& v2,
                   const glm::vec2& v3,
                   const glm::vec2& v4);

  static void Line(const glm::vec2& from,
                   const glm::vec2 to,
                   float thickness = 1.0f);

protected:
  static void Init();
  static void Begin();
  static void End();
};
