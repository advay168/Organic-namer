#include "Quad.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"

static std::vector<glm::vec2> quadPoints;
static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;

void QuadRenderer::Init()
{
  shader = new Shader("res/shaders/quad.vert", "res/shaders/quad.frag");
  layout.add(GL_FLOAT, 2, "aVertex");
  VAO = new VertexArray;
  VAO->Bind();
  VBO = new VertexBuffer(layout);
}

void QuadRenderer::Quad(const glm::vec2& v1,
                        const glm::vec2& v2,
                        const glm::vec2& v3,
                        const glm::vec2& v4)
{
  quadPoints.push_back(v1);
  quadPoints.push_back(v2);
  quadPoints.push_back(v3);
  quadPoints.push_back(v1);
  quadPoints.push_back(v4);
  quadPoints.push_back(v3);
}

void QuadRenderer::Line(const glm::vec2& from,
                        const glm::vec2 to,
                        float thickness)
{
  glm::vec2 offset = { from.y - to.y, to.x - from.x };
  offset = glm::normalize(offset) * thickness;
  Quad(from + offset, from - offset, to - offset, to + offset);
}

void QuadRenderer::Begin()
{
  quadPoints.clear();
}

void QuadRenderer::End()
{
  shader->Bind();
  VAO->Bind();

  VBO->setData(quadPoints.data(), quadPoints.size() * sizeof(quadPoints[0]));
  glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

  shader->setMat4("projection", projection);
  glDrawArrays(GL_TRIANGLES, 0, quadPoints.size());
}
