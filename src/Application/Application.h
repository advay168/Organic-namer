#pragma once
#include "Common.h"
#include "Config.h"

#include "Graphics/Screen.h"

#include "Atom.h"
#include "Bond.h"
#include "Camera.h"
#include "Element.h"
#include "InputState.h"
#include "Namer.h"
#include "PhysicsFormatter.h"
#include "Scene.h"
#include "SelectionBox.h"

class Application
{
public:
    Application(GLFWwindow* window);

    void runFrame();

private:
    void processInput();
    void handleAtomsInput();
    void selectAtomWithin(const glm::vec2& start, const glm::vec2& end);

    void updateFrame();
    void drawFrame();
    void imGuiFrame();
    void displayZoomOptions();
    void displayElements();
    void displayDeletionOptions();
    void displayNamer();

    void createBond(Atom* a, Atom* b);
    void deleteAtom(Atom* atomToDel);
    void deleteBond(Bond* bondToDel);

    std::pair<glm::vec2, glm::vec2> calculateAtomsBoundingBox();

    Atom* findHoveredAtom();
    void bringAtomsIntoView();
    void calcWindowSize();
    void calcCursorPos();
    void setInputState();

private:
    GLFWwindow* window;

    Screen screen;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float width;
    float height;
    float scaling;

    // fonts
    ImFont* normalFont;
    ImFont* largeFont;

    Camera camera;

    InputState inputState;

    Scene currentScene;

    std::array<ElementType, 4> elementsList = { ElementType::Hydrogen, ElementType::Carbon, ElementType::Nitrogen, ElementType::Oxygen };

    Atom* selectedAtom = nullptr;
    bool selectedAtomFollowMouse = false;

    // std::vector<std::pair<std::string, std::string>> elementsList;
    int selectedTmpAtom = -1;
    std::unique_ptr<Atom> tmpAtom;

    SelectionBox selectionBox;

    PhysicsFormatter physicsFormatter;
    bool simulating = false;

    Namer* namer;
    std::string compoundName = "";
};
