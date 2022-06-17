#include "Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Graphics/WindowData.h"
#include "Renderer/Renderer.h"
#include "imgui.h"

int offset = rand();
Application::Application(GLFWwindow* window)
  : window(window)
{
  setCallbacks();
  Renderer::Init();

  elementsList.emplace_back("Hydrogen", "H");
  elementsList.emplace_back("Helium", "He");
  elementsList.emplace_back("Lithium", "Li");
}

void Application::processInput()
{
  handleAtomsInput();
}

void Application::handleAtomsInput()
{
  if (keyPressed[ImGuiKey_Escape]) {
    selectedAtom = nullptr;
    selectedAtomFollowMouse = false;
    selectedTmpAtom = -1;
    tmpAtom.reset();
    return;
  }

  if (outOfWindow)
    return;
  if (!leftMouseClicked)
    return;
  Atom* atom = findHoveredAtom();
  if (keyPressed[ImGuiKey_LeftCtrl]) {
    if (!atom)
      return;
    if (selectedAtom) {
      bonds.emplace_back(selectedAtom, atom);
      selectedAtom = nullptr;
    } else {
      selectedAtom = atom;
    }
    // atom->selected = true;
  } else if (tmpAtom) {
    atoms.push_back(*tmpAtom); // not same atom as vector creates copy
  } else {
    if (!atom)
      return;
    if (selectedAtom) {
      selectedAtom = nullptr;
      selectedAtomFollowMouse = false;
    } else {
      selectedAtom = atom;
      selectedAtomFollowMouse = true;
    }
  }
}

Atom* Application::findHoveredAtom()
{
  for (Atom& atom : atoms) {
    if (atom.isIntersecting(mousePos)) {
      return &atom;
    }
  }
  return nullptr;
}

void Application::updateFrame()
{
  if (selectedAtom && selectedAtomFollowMouse)
    selectedAtom->pos = mousePos;
  if (tmpAtom) {
    tmpAtom->pos = mousePos;
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

  // Renderer::Line({200, 300},{400, 500});
  // Renderer::Quad({ 100, 200 }, { 200, 300 }, { 80, 150 }, { 230, 640 });
  Renderer::Line({ 100, 200 }, { 300, 400 }, 2.0f);
}

void Application::ImGuiFrame()
{
  ImGui::Begin("Application");
  calcWindowSize();
  calcCursorPos();
  setInputState();
  windowFocused = ImGui::IsWindowFocused();

  ImGui::Image((void*)(intptr_t)screen.textureColorbuffer,
               { width, height },
               { 0.0f, 1.0f },
               { 1.0f, 0.0f });

  if (selectedAtom && selectedAtomFollowMouse) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
  }

  ImGui::End();

  ImGui::Begin("MyMainWindow");
  ImGui::Text("The UI");
  ImGui::Text("windowFocused: %i", windowFocused);
  ImGui::Text("outOfWindow: %i", outOfWindow);

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

  if (selectedAtom) {
    ImGui::Text("%s", selectedAtom->name.c_str());
    auto& atomBonds = selectedAtom->bonds;
    for (Bond* bond : atomBonds) {
      if (ImGui::SmallButton(
            ("Delete bond to " + bond->other(selectedAtom).name).c_str())) {
        (void)0;
      }
    }
  }

  ImGui::End();

  ImGui::ShowMetricsWindow();
}

void Application::runFrame()
{
  float currentFrame = glfwGetTime();
  deltaTime = lastFrame - lastFrame;
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
    outOfWindow = true;
    localCoord = glm::clamp(localCoord, { 0.0f, 0.0f }, { 1.0f, 1.0f });
  } else {
    outOfWindow = false;
  }
  localCoord.y = 1.0f - localCoord.y;
  mousePos = localCoord * glm::vec2(WIDTH, HEIGHT);
}

void Application::setInputState()
{
  leftMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
  rightMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Right);
  for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_COUNT; key++) {
    keyPressed[key] = ImGui::IsKeyDown(key);
  }
}

void Application::setCallbacks()
{
  glfwSetFramebufferSizeCallback(
    window, [](GLFWwindow* window, int width, int height) {
      WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
      windowData.application->framebuffer_size_callback(width, height);
    });
}

void Application::framebuffer_size_callback(int width, int height)
{
  glViewport(0, 0, width, height);
}