#pragma once

#include <string>

#include "glm/vec4.hpp"

class Texture
{
private:
  unsigned int ID;

public:
  Texture(const std::string& filePath);

  void Bind(int slot = 0);
};