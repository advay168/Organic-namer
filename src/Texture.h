#pragma once

#include "Common.h"

class Texture
{
private:
  unsigned int ID;

public:
  Texture(const std::string& filePath);

  Texture(unsigned int format, int width, int height, void* data);

  void Bind(int slot = 0);
};