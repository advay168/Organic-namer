#pragma once
#include "Common.h"

#include "Circle.h"
#include "Line.h"
#include "Quad.h"
#include "Text.h"
#include "TextCircle.h"

class Renderer
  : public LineRenderer
  , public QuadRenderer
  , public CircleRenderer
  , public TextRenderer
  , public TextCircleRenderer
{
public:
  static void Init();

  static void Begin();

  static void End();

  static void setViewMatrix(const glm::mat4& viewMatrix);
  static void setProjectionMatrix(const glm::mat4& projectionMatrix);
};
