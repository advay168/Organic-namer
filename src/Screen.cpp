#include "Screen.h"

Screen::Screen(unsigned int& width, unsigned int& height)
  : screenShader("res/shaders/screen.vert", "res/shaders/screen.frag")
  , width(width)
  , height(height)
{
  glViewport(0, 0, width, height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
  glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now
  glViewport(0, 0, WIDTH, HEIGHT);
}

void Screen::draw()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  float virtualAspect = float(WIDTH) / HEIGHT;
  float screenAspect = float(width) / height;

  if (screenAspect > virtualAspect) {
    float delta = width - virtualAspect * height;
    delta /= 2.0;
    glViewport(delta, 0, width - 2 * delta, height);
  } else {
    float delta = height - width / virtualAspect;
    delta /= 2.0;
    glViewport(0, delta, width, height - 2 * delta);
  }

  screenShader.Bind();
  glBindVertexArray(quadVAO);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glViewport(0, 0, width, height);
}
