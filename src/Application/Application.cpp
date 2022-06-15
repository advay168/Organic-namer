#include "Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Graphics/WindowData.h"
#include "Renderer/Renderer.h"

int offset = rand();
Application::Application(GLFWwindow* window)
  : window(window)
{
  setCallbacks();
  Renderer::Init();
  hydrogen.setPos({ 750, 200 });
  helium.setPos({ 1000, 800 });
  atoms.push_back(hydrogen);
  atoms.push_back(helium);
}

void Application::processInput()
{
  if (!window_focused)
    return;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Application::updateFrame()
{
  for (Atom& atom : atoms)
    atom.update(mousePos);
}

void Application::drawFrame()
{
  for (Atom& atom : atoms)
    atom.draw();
}

void Application::ImGuiFrame()
{
  ImGui::Begin("Application");
  calcWindowSize();
  calcCursorPos();
  window_focused = ImGui::IsWindowFocused();

  ImGui::Image((void*)(intptr_t)screen.textureColorbuffer,
               { width, height },
               { 0.0f, 1.0f },
               { 1.0f, 0.0f });
  ImGui::End();

  ImGui::Begin("MyMainWindow");
  ImGui::Text("The UI");
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
  localCoord = glm::clamp(localCoord, { 0.0f, 0.0f }, { 1.0f, 1.0f });
  localCoord.y = 1.0f - localCoord.y;
  mousePos = localCoord * glm::vec2(WIDTH, HEIGHT);
}

void Application::setCallbacks()
{
  glfwSetFramebufferSizeCallback(
    window, [](GLFWwindow* window, int width, int height) {
      WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
      windowData.application->framebuffer_size_callback(width, height);
    });
  glfwSetMouseButtonCallback(
    window, [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
      windowData.application->mouse_button_callback(button, action, mods);
    });
}

void Application::framebuffer_size_callback(int width, int height)
{
  glViewport(0, 0, width, height);
}

void Application::mouse_button_callback(int button, int action, int mods)
{
  (void)mods;
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      for (Atom& atom : atoms) {
        if (atom.isIntersecting(mousePos)) {
          atom.selected = true;
          break;
        }
      }
    } else {
      for (Atom& atom : atoms) {
        atom.selected = false;
      }
    }
  }
}
