#pragma once
#include "Common.h"

class VertexArray
{
private:
  unsigned int ID;

public:
  VertexArray();
  ~VertexArray();

  void Bind();
};
