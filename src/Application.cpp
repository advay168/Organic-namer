// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Circle.h"

struct WindowData
{
  Camera* camera;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  unsigned int width = 800;
  unsigned int height = 600;
  float lastX = 0;
  float lastY = 0;
  bool firstMouse = true;
};

void
processInput(GLFWwindow* window)
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    windowData.camera->ProcessKeyboard(Direction::FORWARD,
                                       windowData.deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    windowData.camera->ProcessKeyboard(Direction::BACKWARD,
                                       windowData.deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    windowData.camera->ProcessKeyboard(Direction::LEFT, windowData.deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    windowData.camera->ProcessKeyboard(Direction::RIGHT, windowData.deltaTime);
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
  windowData.width = width;
  windowData.height = height;
  glViewport(0, 0, width, height);
}

void
mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

  if (windowData.firstMouse) {
    windowData.lastX = xpos;
    windowData.lastY = ypos;
    windowData.firstMouse = false;
  }

  float xoffset = xpos - windowData.lastX;
  float yoffset = windowData.lastY -
                  ypos; // reversed since y-coordinates go from bottom to top

  windowData.lastX = xpos;
  windowData.lastY = ypos;

  windowData.camera->ProcessMouseMovement(xoffset, yoffset);
}

void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
  windowData.camera->ProcessMouseScroll(yoffset);
}

int
main()
{
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  WindowData windowData;
  GLFWwindow* window = glfwCreateWindow(
    windowData.width, windowData.height, "Learn OpenGL", NULL, NULL);
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
  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, +0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    +0.5f, +0.5f, -0.5f, 1.0f, 1.0f, +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, +0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, +0.5f, 0.0f, 0.0f, +0.5f, -0.5f, +0.5f, 1.0f, 0.0f,
    +0.5f, +0.5f, +0.5f, 1.0f, 1.0f, +0.5f, +0.5f, +0.5f, 1.0f, 1.0f,
    -0.5f, +0.5f, +0.5f, 0.0f, 1.0f, -0.5f, -0.5f, +0.5f, 0.0f, 0.0f,

    -0.5f, +0.5f, +0.5f, 1.0f, 0.0f, -0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, +0.5f, 0.0f, 0.0f, -0.5f, +0.5f, +0.5f, 1.0f, 0.0f,

    +0.5f, +0.5f, +0.5f, 1.0f, 0.0f, +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
    +0.5f, -0.5f, -0.5f, 0.0f, 1.0f, +0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    +0.5f, -0.5f, +0.5f, 0.0f, 0.0f, +0.5f, +0.5f, +0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, +0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    +0.5f, -0.5f, +0.5f, 1.0f, 0.0f, +0.5f, -0.5f, +0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, +0.5f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, +0.5f, -0.5f, 0.0f, 1.0f, +0.5f, +0.5f, -0.5f, 1.0f, 1.0f,
    +0.5f, +0.5f, +0.5f, 1.0f, 0.0f, +0.5f, +0.5f, +0.5f, 1.0f, 0.0f,
    -0.5f, +0.5f, +0.5f, 0.0f, 0.0f, -0.5f, +0.5f, -0.5f, 0.0f, 1.0f
  };

  VertexArray VAO;

  Layout layout;
  layout.add(GL_FLOAT, 3, "aPos").add(GL_FLOAT, 2, "aTexCoord");

  VertexBuffer VBO(layout, vertices, sizeof(vertices));

  // Shader shader("res/shaders/basic.vert", "res/shaders/basic.frag");
  Texture tex("res/textures/wall.jpg");

  Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
  windowData.camera = &camera;

  VertexArray lightVAO;
  VBO.Bind();
  VBO.setAttrib();
  Shader lightingShader("res/shaders/lighting.vert",
                        "res/shaders/lighting.frag");
  Shader lightCubeShader("res/shaders/lightCube.vert",
                         "res/shaders/lightCube.frag");
  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

  Circle circ;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    windowData.deltaTime = currentFrame - windowData.lastFrame;
    windowData.lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    VAO.Bind();
    // shader.Bind();
    // tex.Bind(0);
    // shader.set("tex", 0);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = camera.ComputeViewMatrix();

    glm::mat4 projection =
      glm::perspective(glm::radians(camera.m_zoom),
                       float(windowData.width) / windowData.height,
                       0.1f,
                       100.0f);

    // shader.set<glm::mat4&>("model", model);
    // shader.set<glm::mat4&>("view", view);
    // shader.set<glm::mat4&>("projection", projection);

    lightingShader.Bind();
    lightingShader.setVec3("objectColor", { 1.0f, 0.5f, 0.31f });
    lightingShader.setVec3("lightColor", { 1.0f, 1.0f, 1.0f });
    lightingShader.set<glm::mat4&>("model", model);
    lightingShader.set<glm::mat4&>("view", view);
    lightingShader.set<glm::mat4&>("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightCubeShader.Bind();
    lightVAO.Bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightCubeShader.set<glm::mat4&>("model", model);
    lightCubeShader.set<glm::mat4&>("view", view);
    lightCubeShader.set<glm::mat4&>("projection", projection);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}