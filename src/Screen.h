#pragma once

#include "Common.h"
#include "Shader.h"

class Screen
{
public:
  Screen(unsigned int& width, unsigned int& height);
  void preDraw();
  void draw();

private:
  Shader screenShader;
  unsigned int &width, &height;
  unsigned int fbo;
  unsigned int quadVAO, textureColorbuffer;
};
