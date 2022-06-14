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

  void calcWindowSize();
  void calcCursorPos();

  void processInput();

  void updateFrame();
  void drawFrame();
  void ImGuiFrame();

private:
  GLFWwindow* window;
  glm::vec3 circles[4];

  Screen screen;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  float width;
  float height;
  glm::vec2 mousePos;
  bool window_focused = false;
};
