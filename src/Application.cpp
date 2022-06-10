#include "Common.h"

#include "Screen.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Renderer.h"

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
  windowData.width = width;
  windowData.height = height;
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

  if (windowData.firstMouse) {
    windowData.lastX = xpos;
    windowData.lastY = ypos;
    windowData.firstMouse = false;
  }

  // float xoffset = xpos - windowData.lastX;
  // float yoffset = windowData.lastY - ypos; // reversed since y-coordinates go
  // from bottom to top

  windowData.lastX = xpos;
  windowData.lastY = ypos;
}

void scroll_callback(GLFWwindow*, double, double)
{
  // WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
}

int main()
{
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  WindowData windowData;
  GLFWwindow* window = glfwCreateWindow(
    windowData.width, windowData.height, "Organic Naming", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }

  glfwSetWindowUserPointer(window, &windowData);
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  Screen screen(window);

  Renderer::Init(window);

  glm::vec3 circles[] = {
    { 500, 600, 70 }, { 900, 300, 200 }, { 1000, 600, 100 }, { 200, 500, 150 }
  };

  int offset = rand();
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    windowData.deltaTime = currentFrame - windowData.lastFrame;
    windowData.lastFrame = currentFrame;
    processInput(window);

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
      circle = glm::clamp(circle, { 0, 0, 0 }, { WIDTH, HEIGHT, 1000000 });
      Renderer::Circle({ circle.x, circle.y }, circle.z);
      acc += offset;
    }

    Renderer::Text("Bottom Left", 0, 0, 1.0f);
    Renderer::Text("Bottom Right", WIDTH - 310, 0, 1.0f);
    Renderer::Text("Top Left", 0, HEIGHT - 80, 1.0f);
    Renderer::Text("Top Right", WIDTH - 250, HEIGHT - 80, 1.0f);

    Renderer::End();
    screen.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
