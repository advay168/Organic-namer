#include "Renderer.h"

void Renderer::Init()
{
  CircleRenderer::Init();
  TextRenderer::Init();
}
void Renderer::Begin()
{
  CircleRenderer::Begin();
  TextRenderer::Begin();
}

void Renderer::End()
{
  CircleRenderer::End();
  TextRenderer::End();
}
