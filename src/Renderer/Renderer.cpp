#include "Renderer.h"

void Renderer::Init()
{
  QuadRenderer::Init();
  CircleRenderer::Init();
  TextRenderer::Init();
}
void Renderer::Begin()
{
  QuadRenderer::Begin();
  CircleRenderer::Begin();
  TextRenderer::Begin();
}

void Renderer::End()
{
  QuadRenderer::End();
  CircleRenderer::End();
  TextRenderer::End();
}
