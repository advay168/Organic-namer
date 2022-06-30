#include "Quad.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"

static std::vector<glm::vec2> quadPoints;
static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;
static glm::mat4 *view, *projection;

void QuadRenderer::Init(glm::mat4* view_, glm::mat4* projection_)
{
  view = view_;
  projection = projection_;
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

void QuadRenderer::Rectangle(const glm::vec2& topLeft,
                             const glm::vec2& bottomRight)
{
  glm::vec2 v1{ topLeft };
  glm::vec2 v2{ topLeft.x, bottomRight.y };
  glm::vec2 v3{ bottomRight };
  glm::vec2 v4{ bottomRight.x, topLeft.y };
  Quad(v1, v2, v3, v4);
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

  shader->setMat4("view", *view);
  shader->setMat4("projection", *projection);
  glDrawArrays(GL_TRIANGLES, 0, quadPoints.size());
}
