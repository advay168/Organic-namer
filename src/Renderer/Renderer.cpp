#include "Renderer.h"

void Renderer::Init()
{
  LineRenderer::Init();
  QuadRenderer::Init();
  CircleRenderer::Init();
  TextRenderer::Init();
}
void Renderer::Begin()
{
  LineRenderer::Begin();
  QuadRenderer::Begin();
  QuadRenderer::Begin();
  CircleRenderer::Begin();
  TextRenderer::Begin();
}

void Renderer::End()
{
  LineRenderer::End();
  QuadRenderer::End();
  QuadRenderer::End();
  CircleRenderer::End();
  TextRenderer::End();
}
