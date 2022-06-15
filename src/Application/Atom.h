#pragma once
#include "Common.h"

class Atom
{
public:
  Atom(const std::string& name, const std::string& symbol);
  Atom(const std::string& name, const std::string& symbol, const glm::vec3& color);

  void setPos(const glm::vec2& to);

  void update(const glm::vec2& mousePos);
  void draw();

  bool isIntersecting(const glm::vec2& mousePos);

public:
  bool selected;
  float radius = 40.0f;
private:
  std::string name;
  std::string symbol;
  glm::vec2 pos;
  glm::vec3 color;
};
