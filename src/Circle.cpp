#include "Circle.h"

#include <vector>

#include "glad/glad.h"
#include "glm/gtx/transform.hpp"

#include "Constants.h"

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
    vertices.push_back(
      { center + glm::vec2{ radius, radius }, center, radius });
    vertices.push_back(
      { center + glm::vec2{ radius, -radius }, center, radius });
    vertices.push_back(
      { center + glm::vec2{ -radius, radius }, center, radius });
    vertices.push_back(
      { center + glm::vec2{ -radius, radius }, center, radius });
    vertices.push_back(
      { center + glm::vec2{ radius, -radius }, center, radius });
    vertices.push_back(
      { center + glm::vec2{ -radius, -radius }, center, radius });
  }

  VBO->setData(vertices.data(), vertices.size() * sizeof(vertices[0]));
  glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

  shader->setMat4("projection", projection);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
