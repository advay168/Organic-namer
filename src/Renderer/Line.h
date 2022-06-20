#pragma once
#include "Common.h"

class LineRenderer
{
public:
  static void Line(const glm::vec2& from,
                   const glm::vec2 to,
                   float thickness = 1.0f);

  static void DashedLine(const glm::vec2& from,
                         const glm::vec2 to,
                         float thickness = 1.0f,
                         float length = 5.0f);

protected:
  static void Init();
  static void Begin();
  static void End();
};
