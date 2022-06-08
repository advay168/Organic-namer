#include "Screen.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/gtx/transform.hpp>

#include "Constants.h"
#include "WindowData.h"

Screen::Screen(GLFWwindow* window)
  : screenShader("res/shaders/screen.vert", "res/shaders/screen.frag")
  , window(window)
{
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glGenTextures(1, &textureColorbuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // attach it to currently bound framebuffer object
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
  glFramebufferRenderbuffer(
    GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  float quadVertices[] = {
    // vertex attributes for a quad that fills the entire screen in Normalized
    // Device Coordinates.
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f
  };
  unsigned int quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Screen::preDraw()
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, WIDTH, HEIGHT);
}

void Screen::draw()
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);

  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float virtualAspect = float(WIDTH) / HEIGHT;
  float screenAspect = float(windowData.width) / windowData.height;

  if (screenAspect > virtualAspect) {
    float delta = windowData.width - virtualAspect * windowData.height;
    delta /= 2.0;
    glViewport(delta, 0, windowData.width - 2 * delta, windowData.height);
  } else {
    float delta = windowData.height - windowData.width / virtualAspect;
    delta /= 2.0;
    glViewport(0, delta, windowData.width, windowData.height - 2 * delta);
  }

  screenShader.Bind();
  glBindVertexArray(quadVAO);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glViewport(0, 0, windowData.width, windowData.height);
}
