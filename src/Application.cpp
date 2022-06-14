#include "Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Renderer.h"

int offset = rand();
Application::Application(GLFWwindow* window)
  : window(window)
{
  setCallbacks();
  Renderer::Init();
  circles[0] = { 500, 600, 70 };
  circles[1] = { 900, 300, 200 };
  circles[2] = { 1000, 600, 100 };
  circles[3] = { 200, 500, 150 };
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
  int acc = 0;
  for (auto& circle : circles) {
    circle.x += glm::perlin(glm::vec2(lastFrame + acc)) * 50;
    circle.y += glm::perlin(glm::vec2(lastFrame * 2 + acc)) * 50;
    circle = glm::clamp(circle,
                        { circle.z, circle.z, 0 },
                        { WIDTH - circle.z, HEIGHT - circle.z, 1000000 });
    acc += offset;
  }
}

void Application::drawFrame()
{
  Renderer::CenteredText("Organic Naming!!!",
                         { WIDTH / 2.0f, HEIGHT / 2.0f },
                         2.5f,
                         { 0.2f, 0.7f, 0.1f });
  Renderer::Text("Bottom Left", { 0, 0 }, 1.0f);
  Renderer::Text("Bottom Right", { WIDTH - 310, 0 }, 1.0f);
  Renderer::Text("Top Left", { 0, HEIGHT - 80 }, 1.0f);
  Renderer::Text("Top Right", { WIDTH - 250, HEIGHT - 80 }, 1.0f);

  for (auto& circle : circles) {
    Renderer::Circle({ circle.x, circle.y }, circle.z, { 0.8, 0.3, 0.2 });
  }

  glm::vec2 center{ WIDTH / 2.0f, HEIGHT / 2.0f };
  center.x += glm::perlin(glm::vec2(lastFrame * 3)) * 500;
  center.y += glm::perlin(glm::vec2(lastFrame * 2)) * 500;

  Renderer::TextCircle(
    { 200, 600 }, 100, glm::vec3(1.0f), "Hello", 1.0f, glm::vec3(0.0f));

  Renderer::BorderCircle(center,
                         (1 + glm::perlin(glm::vec2(lastFrame))) * 300,
                         { 0.5f, 0.3f, 0.9f },
                         10,
                         { 0.1f, 0.89f, 0.66f });
  Renderer::Circle(mousePos, 30, { 0.23f, 0.35f, 0.46f });
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
}

void Application::framebuffer_size_callback(int width, int height)
{
  glViewport(0, 0, width, height);
}
