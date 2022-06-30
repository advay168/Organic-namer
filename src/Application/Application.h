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
  void handleSelectBoxInput();

  void selectFromSelectBox();

  Atom* findHoveredAtom();

  void createBond(Atom* a, Atom* b);

  void deleteAtom(Atom* atomToDel);
  void deleteBond(Bond* bondToDel);

  std::pair<glm::vec2, glm::vec2> calculateAtomsBoundingBox();

  void bringAtomsIntoView();

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
  float zoom = 1.0f;
  glm::vec3 cameraPos{ WIDTH / 2.0f, HEIGHT / 2.0f, 0.0f };
  glm::mat4 view{ 1.0f };
  glm::mat4 projection{ 1.0f };

  bool leftMouseClicked = false;
  bool leftMousePressed = false;
  bool rightMouseClicked = false;
  bool rightMousePressed = false;
  bool keyPressed[ImGuiKey_COUNT] = { 0 };

  std::list<Atom> atoms;
  Atom* selectedAtom = nullptr;
  bool selectedAtomFollowMouse = false;

  std::vector<std::pair<std::string, std::string>> elementsList;
  int selectedTmpAtom = -1;
  std::unique_ptr<Atom> tmpAtom;

  std::list<Bond> bonds;

  glm::vec2 selectionBoxStart;
  glm::vec2 selectionBoxEnd;
  bool isSelecting = false;
};
