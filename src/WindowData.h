#pragma once

struct WindowData
{
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  unsigned int width = 800;
  unsigned int height = 600;
  float lastX = 0;
  float lastY = 0;
  bool firstMouse = true;
};