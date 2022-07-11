#pragma once

struct InputState
{
  glm::vec2 mousePos;
  bool windowFocused = false;
  bool outOfWindow = true;

  bool leftMouseClicked = false;
  bool leftMousePressed = false;
  bool rightMouseClicked = false;
  bool rightMousePressed = false;
  bool keyPressed[ImGuiKey_COUNT] = { 0 };

  inline bool isKeyPressed(int key) { return keyPressed[key]; }
};
