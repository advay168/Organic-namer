#pragma once

#include "Common.h"
#include "Shader.h"

class Screen
{
public:
  Screen(GLFWwindow* window);
  void preDraw();
  void draw();

private:
  Shader screenShader;
  GLFWwindow* window;
  unsigned int fbo;
  unsigned int quadVAO, textureColorbuffer;
};