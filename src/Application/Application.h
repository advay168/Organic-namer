#pragma once
#include "Common.h"

#include "Graphics/Screen.h"

#include "Atom.h"

class Application
{
public:
  Application(GLFWwindow* window);

  void runFrame();

private:
  void setCallbacks();

  void framebuffer_size_callback(int width, int height);
  void mouse_button_callback(int button, int action, int mods);

  void calcWindowSize();
  void calcCursorPos();

  void processInput();

  void updateFrame();
  void drawFrame();
  void ImGuiFrame();

private:
  GLFWwindow* window;

  Screen screen;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  float width;
  float height;
  glm::vec2 mousePos;
  bool windowFocused = false;
  bool outOfWindow = true;

  std::vector<Atom> atoms;
  std::vector<std::pair<std::string, std::string>> atomsList;
  int selectedAtom = -1;
  std::unique_ptr<Atom> tmpAtom;
};
