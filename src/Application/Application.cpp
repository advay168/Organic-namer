#include "Application.h"

#include <fstream>
#include <imgui.h>
#include <random>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "glm/gtc/random.hpp"
#include "glm/gtx/transform.hpp"

#include "Graphics/WindowData.h"
#include "Renderer/Renderer.h"

std::default_random_engine rng;
Application::Application(GLFWwindow* window)
    : window(window)
    , camera(inputState)
    , selectionBox(inputState)
    , physicsFormatter(currentScene)
{
    Renderer::Init();

    ImGuiIO& io = ImGui::GetIO();
    const char* fontFileName = "res/fonts/NotoSansMono_SemiCondensed-Regular.ttf";
    glfwGetWindowContentScale(window, &scaling, nullptr);
    normalFont = io.Fonts->AddFontFromFileTTF(fontFileName, 18.0f * scaling);
    largeFont = io.Fonts->AddFontFromFileTTF(fontFileName, 30.0f * scaling);
}

void Application::runFrame()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput();
    updateFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
    ImGui::PushFont(normalFont);

    screen.Bind();
    Renderer::Begin();
    Renderer::setViewMatrix(camera.getViewMatrix());
    Renderer::setProjectionMatrix(camera.getProjectionMatrix());
    drawFrame();
    Renderer::End();
    screen.unBind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    imGuiFrame();

    ImGui::PopFont();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Application::processInput()
{
    handleAtomsInput();
    camera.processInput(deltaTime);
    if (!(selectedAtom || tmpAtom))
        selectionBox.processInput(deltaTime);
    if (selectionBox.shouldSelectAtoms)
    {
        selectionBox.shouldSelectAtoms = false;
        const auto& [start, end] = selectionBox.getSelectionBox();
        selectAtomWithin(start, end);
        if (inputState.isKeyPressed(ImGuiKey_LeftCtrl))
        {
            selectedAtomFollowMouse = true;
        }
    }
}

void Application::handleAtomsInput()
{
    if (inputState.isKeyPressed(ImGuiKey_Escape))
    {
        selectedAtom = nullptr;
        selectedAtomFollowMouse = false;
        selectedTmpAtom = -1;
        tmpAtom.reset();
        return;
    }

    if (inputState.isKeyPressed(ImGuiKey_Delete))
    {
        if (selectedAtom && !selectedAtomFollowMouse)
            deleteAtom(selectedAtom);
    }

    if (inputState.outOfWindow)
        return;

    if (!inputState.leftMouseClicked)
        return;

    if (tmpAtom)
    {
        currentScene.atoms.push_back(
            *tmpAtom); // not same atom as vector creates copy
        return;
    }

    if (selectedAtom && selectedAtomFollowMouse)
    {
        selectedAtom = nullptr;
        selectedAtomFollowMouse = false;
        return;
    }

    Atom* atom = findHoveredAtom();
    if (!atom)
    {
        selectedAtom = nullptr;
        return;
    }

    if (selectedAtom && atom != selectedAtom)
    {
        createBond(selectedAtom, atom);
        selectedAtom = nullptr;
        return;
    }

    selectedAtom = atom;

    if (inputState.isKeyPressed(ImGuiKey_LeftCtrl))
    {
        selectedAtomFollowMouse = true;
    }
}

void Application::selectAtomWithin(const glm::vec2& start,
                                   const glm::vec2& end)
{
    for (Atom& atom : currentScene.atoms)
    {
        if (atom.isIntersecting(start, end))
        {
            selectedAtom = &atom;
            return;
        }
    }
}

void Application::updateFrame()
{
    if (selectedAtom && selectedAtomFollowMouse)
        selectedAtom->pos = inputState.mousePos;
    if (tmpAtom)
    {
        tmpAtom->pos = inputState.mousePos;
    }
    if (simulating)
    {
        if (physicsFormatter.applyForce())
            simulating = false;
    }
}

void Application::drawFrame()
{
    for (Atom& atom : currentScene.atoms)
        atom.draw(selectedAtom == &atom);
    if (tmpAtom)
        tmpAtom->draw(false);

    for (Bond& bond : currentScene.bonds)
        bond.draw();

    selectionBox.draw();
}

void Application::imGuiFrame()
{
    {
        ImGui::Begin("Application");
        calcWindowSize();
        calcCursorPos();
        setInputState();
        inputState.windowFocused = ImGui::IsWindowFocused();

        ImGui::Image((void*)(intptr_t)screen.textureColorbuffer, { width, height },
                     { 0.0f, 1.0f }, { 1.0f, 0.0f });

        if (selectedAtom && selectedAtomFollowMouse)
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
        }
        displayZoomOptions();
        ImGui::End();
    } // Application window

    {
        ImGui::Begin("Controls");
        DISPLAY(inputState.windowFocused);
        DISPLAY(inputState.outOfWindow);
        DISPLAY(deltaTime);
        int framesPerSecond = 1.0f / deltaTime;
        DISPLAY(framesPerSecond);
        DISPLAY(currentScene.atoms.size());
        camera.debugDisplay();

        ImGui::Separator();

        if (ImGui::Button("Save scene"))
        {
            std::ofstream file(SCENE_FILE_NAME);
            currentScene.serialize(file);
        }

        if (ImGui::Button("Load scene"))
        {
            std::ifstream file(SCENE_FILE_NAME);
            currentScene.deserialize(file);
        }

        displayElements();

        ImGui::Separator();

        if (ImGui::Button("Bring all atoms into view"))
        {
            bringAtomsIntoView();
        }
        if (ImGui::Button("Randomise Positions"))
        {
            for (Atom& atom : currentScene.atoms)
            {
                atom.pos = glm::linearRand(glm::vec2(0, 0), glm::vec2(WIDTH, HEIGHT));
            }
        }

        if (ImGui::Button("Randomise Bond orders"))
        {
            for (Atom& atom : currentScene.atoms)
            {
                std::shuffle(atom.bonds.begin(), atom.bonds.end(), rng);
            }
        }

        if (ImGui::Checkbox("Simulation", &simulating) && simulating)
            physicsFormatter.reArrangeBonds();

        if (simulating)
        {
            ImGui::DragFloat("Sensitivity in movement", &physicsFormatter.simulationSpeed, 0.0001f, 0.0f, 5.0f);
            ImGui::Text("Difference between current and ideal positions:  %f", physicsFormatter.deltaLength);
        }

        displayDeletionOptions();

        ImGui::Separator();

        displayNamer();

        ImGui::End();
    } // Controls menu

    ImGui::ShowMetricsWindow();
}

void Application::displayZoomOptions()
{
    ImGui::SetCursorPos({ width - 30.0f * scaling, height - 45.0f * scaling});
    ImGui::BeginGroup();
    glm::vec2 oldCursorPos = ImGui::GetCursorPos();
    if (ImGui::Button("Home"))
    {
        camera.Home();
    }
    ImGui::SameLine();
    float HomeWidth = ImGui::GetCursorPos().x - oldCursorPos.x - 4.0f * scaling;
    ImGui::Dummy({});
    ImGui::Indent((HomeWidth - 20.0f * scaling) / 2.0f);
    if (ImGui::Button("-", { 20.0f * scaling, 20.0f *scaling}))
    {
        camera.ZoomOut();
    }
    if (ImGui::Button("+", { 20.0f * scaling, 20.0f *scaling}))
    {
        camera.ZoomIn();
    }
    ImGui::EndGroup();
}

void Application::displayElements()
{
    ImGui::Text("Elements: ");
    ImGui::SameLine();
    if (ImGui::BeginCombo("###atomCombo",
                          selectedTmpAtom == -1
                              ? "Select"
                              : elementsList[selectedTmpAtom].name()))
    {
        for (size_t i = 0; i < elementsList.size(); i++)
        {
            if (ImGui::Selectable(elementsList[i].name(),
                                  (int)i == selectedTmpAtom))
            {
                if ((int)i == selectedTmpAtom)
                {
                    selectedTmpAtom = -1;
                    tmpAtom.reset();
                }
                else
                {
                    selectedTmpAtom = i;
                    tmpAtom.reset(new Atom(elementsList[i]));
                }
            }
        }
        ImGui::EndCombo();
    }
}

void Application::displayDeletionOptions()
{
    if (selectedAtom)
    {
        ImGui::Text("%s", selectedAtom->name.c_str());
        if (ImGui::Button("Delete Atom"))
        {
            deleteAtom(selectedAtom);
        }
        else
        {
            auto& atomBonds = selectedAtom->bonds;
            Bond* bondToDel = nullptr;
            for (Bond* bond : atomBonds)
            {
                std::stringstream name;
                name << "Delete 1 bond to " << bond->other(selectedAtom).name << "##"
                     << bond;
                if (ImGui::Button(name.str().c_str()))
                    bondToDel = bond;
            }
            if (bondToDel)
            {
                bondToDel->count--;
                if (bondToDel->count == 0)
                    deleteBond(bondToDel);
            }
        }
    }
}

void Application::displayNamer()
{
    if (ImGui::Button("Name Compound"))
    {
        namer = new Namer(currentScene);
        compoundName = namer->getName();
        delete namer;
    }
    if (compoundName.empty())
        return;
    ImGui::Text("Name of compound: ");
    ImGui::SameLine();
    ImGui::PushFont(largeFont);
    ImGui::Text("%s", compoundName.c_str());
    ImGui::PopFont();
}

void Application::createBond(Atom* a, Atom* b)
{
    Bond tmpBond(a, b);
    for (Bond& bond : currentScene.bonds)
    {
        if (bond == tmpBond)
        {
            ++bond.count;
            return;
        }
    }
    currentScene.bonds.push_back(tmpBond);
    if (simulating)
        physicsFormatter.reArrangeBonds();
}

void Application::deleteAtom(Atom* atomToDel)
{
    while (atomToDel->bonds.size())
    {
        deleteBond(atomToDel->bonds[0]);
    }
    currentScene.atoms.erase(
        std::find_if(currentScene.atoms.begin(), currentScene.atoms.end(),
                     [atomToDel](Atom& a)
                     { return &a == atomToDel; }));
    if (selectedAtom == atomToDel)
        selectedAtom = nullptr;
}

void Application::deleteBond(Bond* bondToDel)
{
    auto it = std::find_if(currentScene.bonds.begin(), currentScene.bonds.end(),
                           [bondToDel](Bond& x)
                           { return &x == bondToDel; });
    currentScene.bonds.erase(it);
}

std::pair<glm::vec2, glm::vec2> Application::calculateAtomsBoundingBox()
{
    if (currentScene.atoms.empty())
    {
        return { { 0.0f, HEIGHT }, { WIDTH, 0.0f } };
    }
    glm::vec2 topRight(0.0f, 0.0f);
    glm::vec2 bottomLeft(WIDTH, HEIGHT);
    for (Atom& atom : currentScene.atoms)
    {
        bottomLeft = glm::min(bottomLeft, atom.pos);
        topRight = glm::max(topRight, atom.pos);
    }
    bottomLeft -= ATOM_RADIUS;
    topRight += ATOM_RADIUS;
    return { { bottomLeft.x, topRight.y }, { topRight.x, bottomLeft.y } };
}

Atom* Application::findHoveredAtom()
{
    for (Atom& atom : currentScene.atoms)
    {
        if (atom.isIntersecting(inputState.mousePos))
        {
            return &atom;
        }
    }
    return nullptr;
}

void Application::bringAtomsIntoView()
{
    auto [topLeft, bottomRight] = calculateAtomsBoundingBox();
    float offset = 5.0f;
    topLeft += glm::vec2(-offset, offset);
    bottomRight += glm::vec2(offset, -offset);
    float rectWidth = bottomRight.x - topLeft.x;
    float rectHeight = topLeft.y - bottomRight.y;

    glm::vec2 rectCentre = (topLeft + bottomRight) / 2.0f;
    camera.setCameraPos({ rectCentre.x, rectCentre.y, 1.0f });

    float virtualAspect = float(WIDTH) / HEIGHT;
    float rectAspect = rectWidth / rectHeight;
    if (rectAspect > virtualAspect)
    {
        camera.setZoom(WIDTH / rectWidth);
    }
    else
    {
        camera.setZoom(HEIGHT / rectHeight);
    }
}

void Application::calcWindowSize()
{
    float virtualAspect = float(WIDTH) / HEIGHT;
    auto [windowWidth, windowHeight] = ImGui::GetContentRegionAvail();
    float screenAspect = windowWidth / windowHeight;
    width = windowWidth;
    height = windowHeight;
    if (screenAspect > virtualAspect)
    {
        float delta = windowWidth - virtualAspect * windowHeight;
        width = windowWidth - delta;
    }
    else
    {
        float delta = windowHeight - windowWidth / virtualAspect;
        height = windowHeight - delta;
    }
}

void Application::calcCursorPos()
{
    glm::vec2 cursorPos = ImGui::GetCursorScreenPos();
    glm::vec2 window_size = glm::vec2(width, height);
    glm::vec2 realMousePos = ImGui::GetMousePos();

    glm::vec2 localCoord = (realMousePos - cursorPos) / window_size;
    if (localCoord != glm::clamp(localCoord, { 0.0f, 0.0f }, { 1.0f, 1.0f }))
    {
        inputState.outOfWindow = true;
        localCoord = glm::clamp(localCoord, { 0.0f, 0.0f }, { 1.0f, 1.0f });
    }
    else
    {
        inputState.outOfWindow = false;
    }
    localCoord.y = 1.0f - localCoord.y;
    inputState.mousePos = localCoord * glm::vec2(WIDTH, HEIGHT);
    inputState.mousePos = camera.getInverseViewMatrix() * glm::vec4(inputState.mousePos.x, inputState.mousePos.y, 0.0f, 1.0f);
}

void Application::setInputState()
{
    inputState.leftMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    inputState.leftMousePressed = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    inputState.rightMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
    inputState.rightMousePressed = ImGui::IsMouseDown(ImGuiMouseButton_Right);
    for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_COUNT; key++)
    {
        inputState.keyPressed[key] = ImGui::IsKeyDown(key);
    }
}
