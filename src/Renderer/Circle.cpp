#include "Circle.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"

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
static glm::mat4 *view, *projection;

void CircleRenderer::Init(glm::mat4* view_, glm::mat4* projection_)
{
    view = view_;
    projection = projection_;
    shader = new Shader("res/shaders/circle.vert", "res/shaders/circle.frag");
    layout.add(GL_FLOAT, 2, "aPos")
        .add(GL_FLOAT, 2, "aCenter")
        .add(GL_FLOAT, 1, "aRadius")
        .add(GL_FLOAT, 3, "aColor");
    VAO = new VertexArray;
    VAO->Bind();
    VBO = new VertexBuffer(layout);
}

void CircleRenderer::Circle(const glm::vec2& center, float radius)
{
    Circle(center, radius, glm::vec3(1.0f));
}

void CircleRenderer::Circle(const glm::vec2& center,
                            float radius,
                            const glm::vec3& color)
{
    circlePoints.push_back(
        { center + glm::vec2 { radius, radius }, center, radius, color });
    circlePoints.push_back(
        { center + glm::vec2 { radius, -radius }, center, radius, color });
    circlePoints.push_back(
        { center + glm::vec2 { -radius, radius }, center, radius, color });
    circlePoints.push_back(
        { center + glm::vec2 { -radius, radius }, center, radius, color });
    circlePoints.push_back(
        { center + glm::vec2 { radius, -radius }, center, radius, color });
    circlePoints.push_back(
        { center + glm::vec2 { -radius, -radius }, center, radius, color });
}

void CircleRenderer::BorderCircle(const glm::vec2& center,
                                  float innerRadius,
                                  const glm::vec3& innerColor,
                                  float borderThickness,
                                  const glm::vec3& borderColor)
{
    Circle(center, innerRadius + borderThickness, borderColor);
    Circle(center, innerRadius, innerColor);
}
void CircleRenderer::Begin()
{
    circlePoints.clear();
}
void CircleRenderer::End()
{
    shader->Bind();
    VAO->Bind();

    VBO->setData(circlePoints.data(),
                 circlePoints.size() * sizeof(circlePoints[0]));

    shader->setMat4("view", *view);
    shader->setMat4("projection", *projection);
    glDrawArrays(GL_TRIANGLES, 0, circlePoints.size());
}
