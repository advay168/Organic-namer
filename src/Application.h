#pragma once

#include "Common.h"

#include "Screen.h"

class Application
{
public:
  Application(GLFWwindow* window);

  void runFrame();

private:
  void setCallbacks();

  void framebuffer_size_callback(int width, int height);
  void mouse_callback(double xpos, double ypos);

  void processInput();

private:
  GLFWwindow* window;
  glm::vec3 circles[4];
  Screen screen;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  unsigned int width = 800;
  unsigned int height = 600;
  float lastX = 0;
  float lastY = 0;
  bool firstMouse = true;
};
