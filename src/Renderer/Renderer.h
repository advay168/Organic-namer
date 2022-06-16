#pragma once
#include "Common.h"

#include "Circle.h"
#include "Quad.h"
#include "Text.h"
#include "TextCircle.h"

class Renderer
  : public QuadRenderer
  , public CircleRenderer
  , public TextRenderer
  , public TextCircleRenderer
{
public:
  static void Init();

  static void Begin();

  static void End();
};
