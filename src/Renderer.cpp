#include "Renderer.h"

GLFWwindow* Renderer::window;
void Renderer::Init(GLFWwindow* window)
{
  Renderer::window = window;
  Renderer::CircleInit();
  Renderer::TextInit();
}
void Renderer::End()
{
  Renderer::CircleDraw();
  Renderer::TextDraw();
}
void Renderer::Begin() {
  Renderer::CircleClear();
  Renderer::TextClear();
}
