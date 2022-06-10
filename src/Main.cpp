#include "Common.h"

#include "Renderer.h"
#include "Screen.h"

#include "Application.h"

int main()
{
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  WindowData windowData;
  GLFWwindow* window =
    glfwCreateWindow(800, 600, "Organic Naming", nullptr, nullptr);
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

  Application application(window);
  windowData.application = &application;

  while (!glfwWindowShouldClose(window)) {
    application.runFrame();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
