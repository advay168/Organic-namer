#pragma once
#include "Common.h"

#include "InputState.h"

class SelectionBox
{
public:
  SelectionBox(InputState& inputState);
  void processInput(float deltaTime);
  void draw();

  std::pair<const glm::vec2&, const glm::vec2&> getSelectionBox();
  bool shouldSelectAtoms = false;

private:
  InputState& inputState;
  glm::vec2 selectionBoxStart;
  glm::vec2 selectionBoxEnd;
  bool isSelecting = false;
};
