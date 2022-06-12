#pragma once

#include "Common.h"
#include "Shader.h"

class Screen
{
public:
  Screen(unsigned int& width, unsigned int& height);
  void Bind();
  void draw();

  unsigned int textureColorbuffer;
private:
  Shader screenShader;
  unsigned int &width, &height;
  unsigned int fbo;
  unsigned int quadVAO;
};
