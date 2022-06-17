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

  float radius = 40.0f;
  std::vector<class Bond*> bonds;

  std::string name;
  std::string symbol;
  glm::vec2 pos;
  glm::vec3 color;

};
