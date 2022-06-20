#include "Line.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"

struct LineVertex
{
  glm::vec2 aVertex;
  glm::vec2 aFrom;
  glm::vec2 aTo;
  float thickness;
};

static std::vector<LineVertex> linePoints;
static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;

void LineRenderer::Init()
{
  shader = new Shader("res/shaders/line.vert", "res/shaders/line.frag");
  layout.add(GL_FLOAT, 2, "aVertex")
    .add(GL_FLOAT, 2, "aFrom")
    .add(GL_FLOAT, 2, "aTo")
    .add(GL_FLOAT, 1, "aThickness");
  VAO = new VertexArray;
  VAO->Bind();
  VBO = new VertexBuffer(layout);
}

void LineRenderer::Line(const glm::vec2& from,
                        const glm::vec2 to,
                        float thickness)
{
  glm::vec2 offset = { from.y - to.y, to.x - from.x };
  offset = glm::normalize(offset) * thickness;
  glm::vec2 v1 = from + offset;
  glm::vec2 v2 = from - offset;
  glm::vec2 v3 = to - offset;
  glm::vec2 v4 = to + offset;
  linePoints.push_back({ v1, from, to, thickness });
  linePoints.push_back({ v2, from, to, thickness });
  linePoints.push_back({ v3, from, to, thickness });
  linePoints.push_back({ v1, from, to, thickness });
  linePoints.push_back({ v4, from, to, thickness });
  linePoints.push_back({ v3, from, to, thickness });
}

void LineRenderer::Begin()
{
  linePoints.clear();
}

void LineRenderer::End()
{
  shader->Bind();
  VAO->Bind();

  VBO->setData(linePoints.data(), linePoints.size() * sizeof(linePoints[0]));
  glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

  shader->setMat4("projection", projection);
  glDrawArrays(GL_TRIANGLES, 0, linePoints.size());
}
