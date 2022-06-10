#include "Renderer.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

struct Vertex
{
  glm::vec2 aPos;
  glm::vec2 aCenter;
  float aRadius;
  glm::vec3 aColor;
};

static std::vector<Vertex> circlePoints;
static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;

void Renderer::CircleInit()
{
  shader = new Shader("res/shaders/circle.vert", "res/shaders/circle.frag");
  layout.add(GL_FLOAT, 2, "aPos")
    .add(GL_FLOAT, 2, "aCenter")
    .add(GL_FLOAT, 1, "aRadius")
    .add(GL_FLOAT, 3, "aColor");
  VAO = new VertexArray;
  VAO->Bind();
  VBO = new VertexBuffer(layout);
}

void Renderer::Circle(const glm::vec2& center, float radius)
{
  Renderer::Circle(center, radius, glm::vec3(1.0f));
}

void Renderer::Circle(const glm::vec2& center,
                      float radius,
                      const glm::vec3& color)
{
  circlePoints.push_back(
    { center + glm::vec2{ radius, radius }, center, radius, color });
  circlePoints.push_back(
    { center + glm::vec2{ radius, -radius }, center, radius, color });
  circlePoints.push_back(
    { center + glm::vec2{ -radius, radius }, center, radius, color });
  circlePoints.push_back(
    { center + glm::vec2{ -radius, radius }, center, radius, color });
  circlePoints.push_back(
    { center + glm::vec2{ radius, -radius }, center, radius, color });
  circlePoints.push_back(
    { center + glm::vec2{ -radius, -radius }, center, radius, color });
}

void Renderer::BorderCircle(const glm::vec2& center,
                            float innerRadius,
                            const glm::vec3& innerColor,
                            float borderThickness,
                            const glm::vec3& borderColor)
{
  Renderer::Circle(center, innerRadius + borderThickness, borderColor);
  Renderer::Circle(center, innerRadius, innerColor);
}
void Renderer::CircleClear()
{
  circlePoints.clear();
}
void Renderer::CircleDraw()
{
  shader->Bind();
  VAO->Bind();

  VBO->setData(circlePoints.data(),
               circlePoints.size() * sizeof(circlePoints[0]));
  glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

  shader->setMat4("projection", projection);
  glDrawArrays(GL_TRIANGLES, 0, circlePoints.size());
}
