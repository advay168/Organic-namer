#pragma once
#include "Common.h"

struct Atom
{
  Atom(const std::string& name, const std::string& symbol);
  Atom(const std::string& name,
       const std::string& symbol,
       const glm::vec3& color);

  void draw(bool selected);

  bool isIntersecting(const glm::vec2& mousePos);

  bool isIntersecting(const glm::vec2& boxStart, const glm::vec2& boxEnd);

  float radius = 40.0f;
  std::vector<struct Bond*> bonds;

  std::string name;
  std::string symbol;
  glm::vec2 pos;
  glm::vec3 color;
};
