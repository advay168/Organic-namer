#pragma once
#include "Common.h"

#include "Graphics/Screen.h"

#include "InputState.h"
#include "Camera.h"
#include "Atom.h"
#include "Bond.h"
#include "SelectionBox.h"

class Application
{
public:
  Application(GLFWwindow* window);

  void runFrame();

private:
  void calcWindowSize();
  void calcCursorPos();
  void setInputState();

  void processInput();

  void updateFrame();
  void drawFrame();
  void ImGuiFrame();

  void handleAtomsInput();
  void handleSelectBoxInput();

  void selectAtomWithin(const glm::vec2& start, const glm::vec2& end);

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

  Camera camera;

  InputState inputState;

  std::list<Atom> atoms;
  Atom* selectedAtom = nullptr;
  bool selectedAtomFollowMouse = false;

  std::vector<std::pair<std::string, std::string>> elementsList;
  int selectedTmpAtom = -1;
  std::unique_ptr<Atom> tmpAtom;

  std::list<Bond> bonds;

  SelectionBox selectionBox;
};
