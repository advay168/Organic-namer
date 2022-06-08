#pragma once

#include "Shader.h"
#include <glfw/glfw3.h>

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