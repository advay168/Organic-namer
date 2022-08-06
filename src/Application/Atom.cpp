#include "Atom.h"

#include "Renderer/Renderer.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/string_cast.hpp"

Atom::Atom(const std::string& name,
           const std::string& symbol,
           const glm::vec3& color)
  : name(name)
  , symbol(symbol)
  , color(color)
{
}

Atom::Atom(const std::string& name, const std::string& symbol)
  : Atom(name, symbol, glm::vec3(1.0f))
{
  size_t hash = std::hash<std::string>{}(name);
  unsigned int r = (hash & 0xff0000) >> 4 * 4;
  unsigned int g = (hash & 0x00ff00) >> 2 * 4;
  unsigned int b = (hash & 0x0000ff) >> 0 * 4;
  color = glm::vec3(r, g, b) / 255.0f;
}

void Atom::draw(bool selected)
{
  float grayscale = (color.r * 0.299 + color.g * 0.587 + color.b * 0.114);
  glm::vec3 color = this->color + (selected ? 0.2f : 0.0f);
  glm::vec3 textColor(grayscale > 186 ? 0.0f : 1.0f);
  if (selected)
    Renderer::BorderCircle(pos, radius, color, 1.0f, glm::vec3(1.0f));
  Renderer::TextCircle(pos, radius, color, symbol, 1.0f, textColor);
}

bool Atom::isIntersecting(const glm::vec2& mousePos)
{
  return glm::distance2(mousePos, pos) < radius * radius;
}

bool Atom::isIntersecting(const glm::vec2& boxStart, const glm::vec2& boxEnd)
{
  glm::vec2 bottomLeft = glm::min(boxStart, boxEnd);
  glm::vec2 topRight = glm::max(boxStart, boxEnd);
  glm::vec2 clamped = glm::clamp(pos, bottomLeft, topRight);
  return isIntersecting(clamped);
}
