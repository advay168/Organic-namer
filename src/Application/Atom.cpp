#include "Atom.h"

#include "Renderer/Renderer.h"
#include "glm/gtx/norm.hpp"

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

void Atom::setPos(const glm::vec2& to)
{
  pos = to;
}

void Atom::update(const glm::vec2& mousePos){
  if(selected)
    pos = mousePos;

}

void Atom::draw()
{
  float grayscale = (color.r * 0.299 + color.g * 0.587 + color.b * 0.114);
  glm::vec3 color = this->color + (selected ? 0.1f : 0.0f);
  if (grayscale > 186)
    Renderer::TextCircle(pos, radius, color, symbol, 1.0f, glm::vec3(0));
  else
    Renderer::TextCircle(pos, radius, color, symbol, 1.0f, glm::vec3(1));
}
bool Atom::isIntersecting(const glm::vec2& mousePos)
{
  return glm::distance2(mousePos, pos) < radius * radius;
}
