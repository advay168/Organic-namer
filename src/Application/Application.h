#pragma once
#include "Common.h"

#include "Graphics/Screen.h"

#include "Atom.h"
#include "Bond.h"

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
  void setInputState();

  void processInput();

  void updateFrame();
  void drawFrame();
  void ImGuiFrame();

  void handleAtomsInput();

  Atom* findHoveredAtom();

  void deleteBond(Bond* bondToDel);

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

  bool leftMouseClicked = false;
  bool rightMouseClicked = false;
  bool keyPressed[ImGuiKey_COUNT] = {0};

  std::list<Atom> atoms;
  Atom* selectedAtom = nullptr;
  bool selectedAtomFollowMouse = false;

  std::vector<std::pair<std::string, std::string>> elementsList;
  int selectedTmpAtom = -1;
  std::unique_ptr<Atom> tmpAtom;

  std::list<Bond> bonds;
};
