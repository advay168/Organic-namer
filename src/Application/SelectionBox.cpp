#include "SelectionBox.h"

#include "Renderer/Renderer.h"
SelectionBox::SelectionBox(InputState& inputState)
    : inputState(inputState)
{
}

void SelectionBox::processInput(float)
{
    if (inputState.outOfWindow)
        return;
    if (inputState.leftMouseClicked)
    {
        selectionBoxStart = selectionBoxEnd = inputState.mousePos;
        isSelecting = true;
    }
    else if (isSelecting)
    {
        if (inputState.leftMousePressed)
        {
            selectionBoxEnd = inputState.mousePos;
        }
        else
        {
            // Selection finished
            isSelecting = false;
            if (selectionBoxStart != selectionBoxEnd)
            {
                shouldSelectAtoms = true;
            }
        }
    }
}

void SelectionBox::draw()
{
    if (isSelecting)
    {
        glm::vec2 v1 { selectionBoxStart };
        glm::vec2 v2 { selectionBoxStart.x, selectionBoxEnd.y };
        glm::vec2 v3 { selectionBoxEnd };
        glm::vec2 v4 { selectionBoxEnd.x, selectionBoxStart.y };
        Renderer::DashedLine(v1, v2, 3.0f, 6.0f);
        Renderer::DashedLine(v2, v3, 3.0f, 6.0f);
        Renderer::DashedLine(v3, v4, 3.0f, 6.0f);
        Renderer::DashedLine(v4, v1, 3.0f, 6.0f);
    }
}
std::pair<const glm::vec2&, const glm::vec2&> SelectionBox::getSelectionBox()
{
    return { selectionBoxStart, selectionBoxEnd };
}
