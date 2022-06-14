#pragma once

#include "Common.h"
#include "Shader.h"

class Screen
{
public:
  Screen();
  void Bind();
  void unBind();

  unsigned int textureColorbuffer;

private:
  Shader screenShader;
  unsigned int fbo;
  unsigned int quadVAO;
};
