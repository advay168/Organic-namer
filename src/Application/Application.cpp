#include "Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Graphics/WindowData.h"
#include "Renderer/Renderer.h"
#include "glm/gtx/transform.hpp"

int offset = rand();
Application::Application(GLFWwindow* window)
  : window(window)
  , camera(inputState)
  , selectionBox(inputState)
{
  Renderer::Init();

  elementsList.emplace_back("Hydrogen", "H");
  elementsList.emplace_back("Helium", "He");
  elementsList.emplace_back("Lithium", "Li");
}

void Application::processInput()
{
  handleAtomsInput();
  camera.processInput(deltaTime);
  if (!(selectedAtom || tmpAtom))
    selectionBox.processInput(deltaTime);
  if (selectionBox.shouldSelectAtoms) {
    selectionBox.shouldSelectAtoms = false;
    const auto& [start, end] = selectionBox.getSelectionBox();
    selectAtomWithin(start, end);
    if (inputState.isKeyPressed(ImGuiKey_LeftCtrl)) {
      selectedAtomFollowMouse = true;
    }
  }
}

void Application::handleAtomsInput()
{
  if (inputState.isKeyPressed(ImGuiKey_Escape)) {
    selectedAtom = nullptr;
    selectedAtomFollowMouse = false;
    selectedTmpAtom = -1;
    tmpAtom.reset();
    return;
  }

  if (inputState.isKeyPressed(ImGuiKey_Delete)) {
    if (selectedAtom && !selectedAtomFollowMouse)
      deleteAtom(selectedAtom);
  }

  if (inputState.outOfWindow)
    return;

  if (!inputState.leftMouseClicked)
    return;

  if (tmpAtom) {
    atoms.push_back(*tmpAtom); // not same atom as vector creates copy
    return;
  }

  if (selectedAtom && selectedAtomFollowMouse) {
    selectedAtom = nullptr;
    selectedAtomFollowMouse = false;
    return;
  }

  Atom* atom = findHoveredAtom();
  if (!atom) {
    selectedAtom = nullptr;
    return;
  }

  if (selectedAtom && atom != selectedAtom) {
    createBond(selectedAtom, atom);
    selectedAtom = nullptr;
    return;
  }

  selectedAtom = atom;

  if (inputState.isKeyPressed(ImGuiKey_LeftCtrl)) {
    selectedAtomFollowMouse = true;
  }
}

void Application::selectAtomWithin(const glm::vec2& start, const glm::vec2& end)
{
  for (Atom& atom : atoms) {
    if (atom.isIntersecting(start, end)) {
      selectedAtom = &atom;
      return;
    }
  }
}

Atom* Application::findHoveredAtom()
{
  for (Atom& atom : atoms) {
    if (atom.isIntersecting(inputState.mousePos)) {
      return &atom;
    }
  }
  return nullptr;
}

void Application::createBond(Atom* a, Atom* b)
{
  Bond tmpBond(a, b);
  for (Bond& bond : bonds) {
    if (bond == tmpBond) {
      ++bond.count;
      return;
    }
  }
  bonds.push_back(tmpBond);
}

void Application::deleteAtom(Atom* atomToDel)
{
  while (atomToDel->bonds.size()) {
    deleteBond(atomToDel->bonds[0]);
  }
  atoms.erase(std::find_if(atoms.begin(), atoms.end(), [atomToDel](Atom& a) {
    return &a == atomToDel;
  }));
  if (selectedAtom == atomToDel)
    selectedAtom = nullptr;
}

void Application::deleteBond(Bond* bondToDel)
{
  auto it = std::find_if(bonds.begin(), bonds.end(), [bondToDel](Bond& x) {
    return &x == bondToDel;
  });
  bonds.erase(it);
}

std::pair<glm::vec2, glm::vec2> Application::calculateAtomsBoundingBox()
{
  if (atoms.empty()) {
    return { { 0.0f, HEIGHT }, { WIDTH, 0.0f } };
  }
  glm::vec2 topRight(0.0f, 0.0f);
  glm::vec2 bottomLeft(WIDTH, HEIGHT);
  for (Atom& atom : atoms) {
    bottomLeft = glm::min(bottomLeft, atom.pos);
    topRight = glm::max(topRight, atom.pos);
  }
  bottomLeft -= atoms.begin()->radius;
  topRight += atoms.begin()->radius;
  return { { bottomLeft.x, topRight.y }, { topRight.x, bottomLeft.y } };
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
  if (rectAspect > virtualAspect) {
    camera.setZoom(WIDTH / rectWidth);
  } else {
    camera.setZoom(HEIGHT / rectHeight);
  }
}

void Application::updateFrame()
{
  if (selectedAtom && selectedAtomFollowMouse)
    selectedAtom->pos = inputState.mousePos;
  if (tmpAtom) {
    tmpAtom->pos = inputState.mousePos;
  }
}

void Application::drawFrame()
{
  for (Atom& atom : atoms)
    atom.draw(selectedAtom == &atom);
  if (tmpAtom)
    tmpAtom->draw(false);

  for (Bond& bond : bonds)
    bond.draw();

  selectionBox.draw();
}

void Application::ImGuiFrame()
{
  ImGui::Begin("Application");
  calcWindowSize();
  calcCursorPos();
  setInputState();
  inputState.windowFocused = ImGui::IsWindowFocused();

  ImGui::Image((void*)(intptr_t)screen.textureColorbuffer,
               { width, height },
               { 0.0f, 1.0f },
               { 1.0f, 0.0f });

  if (selectedAtom && selectedAtomFollowMouse) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
  }

  ImGui::SetCursorPos({ width - 30.0f, height - 45.0f });
  ImGui::BeginGroup();
  glm::vec2 oldCursorPos = ImGui::GetCursorPos();
  if (ImGui::Button("Home")) {
    camera.Home();
  }
  ImGui::SameLine();
  float HomeWidth = ImGui::GetCursorPos().x - oldCursorPos.x - 4.0f;
  ImGui::Dummy({});
  ImGui::Indent((HomeWidth - 20.0f) / 2.0f);
  if (ImGui::Button("-", { 20.0f, 20.0f })) {
    camera.ZoomOut();
  }
  if (ImGui::Button("+", { 20.0f, 20.0f })) {
    camera.ZoomIn();
  }
  ImGui::EndGroup();
  ImGui::End();

  ImGui::Begin("Controls");
  ImGui::Text("The UI");
  DISPLAY(inputState.windowFocused);
  DISPLAY(inputState.outOfWindow);
  DISPLAY(deltaTime);
  camera.debugDisplay();

  ImGui::Separator();

  ImGui::Text("Elements: ");
  ImGui::SameLine();
  if (ImGui::BeginCombo("###atomCombo",
                        selectedTmpAtom == -1
                          ? "Select"
                          : elementsList[selectedTmpAtom].first.c_str())) {
    for (size_t i = 0; i < elementsList.size(); i++) {
      if (ImGui::Selectable(elementsList[i].first.c_str(),
                            (int)i == selectedTmpAtom)) {
        if ((int)i == selectedTmpAtom) {
          selectedTmpAtom = -1;
          tmpAtom.reset();
        } else {
          selectedTmpAtom = i;
          tmpAtom.reset(
            new Atom(elementsList[i].first, elementsList[i].second));
        }
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Button("Bring all atoms into view")) {
    bringAtomsIntoView();
  }

  if (selectedAtom) {
    ImGui::Text("%s", selectedAtom->name.c_str());
    if (ImGui::Button("Delete Atom")) {
      deleteAtom(selectedAtom);
    } else {
      auto& atomBonds = selectedAtom->bonds;
      Bond* bondToDel = nullptr;
      for (Bond* bond : atomBonds) {
        std::stringstream name;
        name << "Delete 1 bond to " << bond->other(selectedAtom).name << "##"
             << bond;
        if (ImGui::Button(name.str().c_str()))
          bondToDel = bond;
      }
      if (bondToDel) {
        bondToDel->count--;
        if (bondToDel->count == 0)
          deleteBond(bondToDel);
      }
    }
  }

  ImGui::End();

  ImGui::ShowMetricsWindow();
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

  screen.Bind();
  Renderer::Begin();
  Renderer::setViewMatrix(camera.getViewMatrix());
  Renderer::setProjectionMatrix(camera.getProjectionMatrix());
  drawFrame();
  Renderer::End();
  screen.unBind();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGuiFrame();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Application::calcWindowSize()
{
  float virtualAspect = float(WIDTH) / HEIGHT;
  auto [windowWidth, windowHeight] = ImGui::GetContentRegionAvail();
  float screenAspect = windowWidth / windowHeight;
  width = windowWidth;
  height = windowHeight;
  if (screenAspect > virtualAspect) {
    float delta = windowWidth - virtualAspect * windowHeight;
    width = windowWidth - delta;
  } else {
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
  if (localCoord != glm::clamp(localCoord, { 0.0f, 0.0f }, { 1.0f, 1.0f })) {
    inputState.outOfWindow = true;
    localCoord = glm::clamp(localCoord, { 0.0f, 0.0f }, { 1.0f, 1.0f });
  } else {
    inputState.outOfWindow = false;
  }
  localCoord.y = 1.0f - localCoord.y;
  inputState.mousePos = localCoord * glm::vec2(WIDTH, HEIGHT);
  inputState.mousePos =
    camera.getInverseViewMatrix() *
    glm::vec4(inputState.mousePos.x, inputState.mousePos.y, 0.0f, 1.0f);
}

void Application::setInputState()
{
  inputState.leftMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
  inputState.leftMousePressed = ImGui::IsMouseDown(ImGuiMouseButton_Left);
  inputState.rightMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
  inputState.rightMousePressed = ImGui::IsMouseDown(ImGuiMouseButton_Right);
  for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_COUNT; key++) {
    inputState.keyPressed[key] = ImGui::IsKeyDown(key);
  }
}
