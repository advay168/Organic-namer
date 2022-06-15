#pragma once
#include "Common.h"

#include "RendererInternals/Circle.h"
#include "RendererInternals/Text.h"
#include "RendererInternals/TextCircle.h"

class Renderer
  : public CircleRenderer
  , public TextRenderer
  , public TextCircleRenderer
{
public:
  static void Init();

  static void Begin();

  static void End();
};
