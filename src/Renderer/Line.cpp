#include "Line.h"

#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBuffer.h"

struct LineVertex
{
    glm::vec2 aPos;
    glm::vec4 aFromTo;
    float aThickness;
    float aDashLength;
};

static std::vector<LineVertex> linePoints;
static Shader* shader;
static VertexArray* VAO;
static Layout layout;
static VertexBuffer* VBO;
static glm::mat4 *view, *projection;

static int frameCount = 0;

void LineRenderer::Init(glm::mat4* view_, glm::mat4* projection_)
{
    view = view_;
    projection = projection_;
    shader = new Shader("res/shaders/line.vert", "res/shaders/line.frag");
    layout.add(GL_FLOAT, 2, "aPos")
        .add(GL_FLOAT, 4, "aFromTo")
        .add(GL_FLOAT, 1, "aThickness")
        .add(GL_FLOAT, 1, "aDashLength");
    VAO = new VertexArray;
    VAO->Bind();
    VBO = new VertexBuffer(layout);
}

void LineRenderer::Line(const glm::vec2& from,
                        const glm::vec2 to,
                        float thickness)
{
    DashedLine(from, to, thickness, 0.0f);
}

void LineRenderer::DashedLine(const glm::vec2& from,
                              const glm::vec2 to,
                              float thickness,
                              float length)
{
    glm::vec2 offset = { from.y - to.y, to.x - from.x };
    offset = glm::normalize(offset) * thickness;
    glm::vec2 v1 = from + offset;
    glm::vec2 v2 = from - offset;
    glm::vec2 v3 = to - offset;
    glm::vec2 v4 = to + offset;
    linePoints.push_back({ v1, { from, to }, thickness, length });
    linePoints.push_back({ v2, { from, to }, thickness, length });
    linePoints.push_back({ v3, { from, to }, thickness, length });
    linePoints.push_back({ v1, { from, to }, thickness, length });
    linePoints.push_back({ v4, { from, to }, thickness, length });
    linePoints.push_back({ v3, { from, to }, thickness, length });
}

void LineRenderer::Begin()
{
    linePoints.clear();
    frameCount++;
}

void LineRenderer::End()
{
    shader->Bind();
    VAO->Bind();

    VBO->setData(linePoints.data(), linePoints.size() * sizeof(linePoints[0]));

    shader->setMat4("view", *view);
    shader->setMat4("projection", *projection);
    shader->setInt("frame", frameCount / 30);
    glDrawArrays(GL_TRIANGLES, 0, linePoints.size());
}
