#include "TextCircle.h"

#include "Renderer.h"

void TextCircleRenderer::TextCircle(const glm::vec2& center,
                                    float radius,
                                    const glm::vec3& circleColor,
                                    const std::string& text,
                                    float scale,
                                    const glm::vec3& textColor)
{
  Renderer::Circle(center, radius, circleColor);
  Renderer::CenteredText(text, center, scale, textColor);
}
