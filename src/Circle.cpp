#include "Circle.h"

#include <vector>

#include "glad/glad.h"
#include "glm/gtx/transform.hpp"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "WindowData.h"

static std::vector<CircleData> circles;
static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;
static GLFWwindow* window;

Circle::Circle()
  : Circle({ 0.0f, 0.0f }, 0.0f)
{
}

Circle::Circle(const glm::vec2& center, float radius)
  : m_data({ center, radius })
{
  circles.push_back(m_data);
}

Circle::~Circle()
{
  circles.erase(std::remove(circles.begin(), circles.end(), m_data),
                circles.end());
}

void Circle::init(GLFWwindow* window_)
{
  shader = new Shader("res/shaders/circle.vert", "res/shaders/circle.frag");
  layout.add(GL_FLOAT, 2, "aPos")
    .add(GL_FLOAT, 2, "aCenter")
    .add(GL_FLOAT, 1, "aRadius");
  VAO = new VertexArray;
  VAO->Bind();
  VBO = new VertexBuffer(layout);
  window = window_;
}

struct Vertex
{
  glm::vec2 aPos;
  glm::vec2 aCenter;
  float aRadius;
};

void Circle::draw()
{
  WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
  shader->Bind();
  VAO->Bind();
  std::vector<Vertex> vertices;
  for (auto& [center, radius] : circles) {
    glm::vec2 normalizedCenter = center * 2.0f - 1.0f;
    float normalizedRadius = radius * 2.0f;
    vertices.push_back(
      { normalizedCenter + glm::vec2{ normalizedRadius, normalizedRadius },
        normalizedCenter,
        normalizedRadius });
    vertices.push_back(
      { normalizedCenter + glm::vec2{ normalizedRadius, -normalizedRadius },
        normalizedCenter,
        normalizedRadius });
    vertices.push_back(
      { normalizedCenter + glm::vec2{ -normalizedRadius, normalizedRadius },
        normalizedCenter,
        normalizedRadius });
    vertices.push_back(
      { normalizedCenter + glm::vec2{ -normalizedRadius, normalizedRadius },
        normalizedCenter,
        normalizedRadius });
    vertices.push_back(
      { normalizedCenter + glm::vec2{ normalizedRadius, -normalizedRadius },
        normalizedCenter,
        normalizedRadius });
    vertices.push_back(
      { normalizedCenter + glm::vec2{ -normalizedRadius, -normalizedRadius },
        normalizedCenter,
        normalizedRadius });
  }

  VBO->setData(vertices.data(), vertices.size() * sizeof(vertices[0]));
  glm::mat4 projection;
  float aspectRatio = float(windowData.width) / float(windowData.height);
  if (aspectRatio >= 1.0f)
    projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);
  else
    projection =
      glm::ortho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio);

  shader->setMat4("projection", projection);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
