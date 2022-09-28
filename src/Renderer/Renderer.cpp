#include "Renderer.h"

#include <deque>

static std::deque<std::function<void()>> debugFuncs;

static glm::mat4 view;
static glm::mat4 projection;

void Renderer::Init()
{
    LineRenderer::Init(&view, &projection);
    QuadRenderer::Init(&view, &projection);
    CircleRenderer::Init(&view, &projection);
    TextRenderer::Init(&view, &projection);
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
    while (debugFuncs.size())
    {
        debugFuncs.back()();
        debugFuncs.pop_back();
    }

    LineRenderer::End();
    QuadRenderer::End();
    QuadRenderer::End();
    CircleRenderer::End();
    TextRenderer::End();
}

void Renderer::DebugRenderFunction(std::function<void()> func)
{
    debugFuncs.push_back(func);
}

void Renderer::setViewMatrix(const glm::mat4& viewMatrix)
{
    view = viewMatrix;
}

void Renderer::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
    projection = projectionMatrix;
}
