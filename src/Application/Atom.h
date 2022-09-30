#pragma once
#include "Common.h"
#include "Config.h"

#include "Element.h"

struct Atom
{
  //Atom(const std::string& name, const std::string& symbol);
  //Atom(const std::string& name,
       //const std::string& symbol,
       //const glm::vec3& color);
  Atom(ElementType element);

  void draw(bool selected);

  bool isIntersecting(const glm::vec2& mousePos);

  bool isIntersecting(const glm::vec2& boxStart, const glm::vec2& boxEnd);

  std::vector<struct Bond*> bonds;

  ElementType element;

  std::string name;
  std::string symbol;
  glm::vec2 pos;
  glm::vec3 color;

  glm::vec2 force{ 0.0f };
};
