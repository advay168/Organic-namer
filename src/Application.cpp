#include "Application.h"

#include "Renderer.h"

int offset = rand();
Application::Application(GLFWwindow* window)
  : window(window)
  , screen(width, height)
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
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Application::runFrame()
{
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  processInput();

  screen.preDraw();
  Renderer::Begin();
  Renderer::Text("Organic Naming!!!",
                 WIDTH / 2.0f - 400,
                 HEIGHT - 200,
                 2.5f,
                 { 0.2f, 0.7f, 0.1f });

  int acc = 0;
  for (auto& circle : circles) {
    circle.x += glm::perlin(glm::vec2(currentFrame + acc)) * 50;
    circle.y += glm::perlin(glm::vec2(currentFrame * 2 + acc)) * 50;
    circle = glm::clamp(circle,
                        { circle.z, circle.z, 0 },
                        { WIDTH - circle.z, HEIGHT - circle.z, 1000000 });
    Renderer::Circle({ circle.x, circle.y }, circle.z, { 0.8, 0.3, 0.2 });
    acc += offset;
  }
  glm::vec2 center{ WIDTH / 2.0f, HEIGHT / 2.0f };
  center.x += glm::perlin(glm::vec2(currentFrame * 3)) * 500;
  center.y += glm::perlin(glm::vec2(currentFrame * 2)) * 500;
  Renderer::BorderCircle(center,
                         (1 + glm::perlin(glm::vec2(currentFrame))) * 300,
                         { 0.5f, 0.3f, 0.9f },
                         10,
                         { 0.1f, 0.89f, 0.66f });

  Renderer::Text("Bottom Left", 0, 0, 1.0f);
  Renderer::Text("Bottom Right", WIDTH - 310, 0, 1.0f);
  Renderer::Text("Top Left", 0, HEIGHT - 80, 1.0f);
  Renderer::Text("Top Right", WIDTH - 250, HEIGHT - 80, 1.0f);

  Renderer::End();
  screen.draw();
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Application::setCallbacks()
{
  glfwSetFramebufferSizeCallback(
    window, [](GLFWwindow* window, int width, int height) {
      WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
      windowData.application->framebuffer_size_callback(width, height);
    });
  glfwSetCursorPosCallback(
    window, [](GLFWwindow* window, double xpos, double ypos) {
      WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
      windowData.application->mouse_callback(xpos, ypos);
    });
}

void Application::framebuffer_size_callback(int width, int height)
{
  this->width = width;
  this->height = height;
  glViewport(0, 0, width, height);
}
void Application::mouse_callback(double xpos, double ypos)
{
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  // reversed since y-coordinates go from bottom to top
  xoffset = yoffset = xoffset;

  lastX = xpos;
  lastY = ypos;
}
