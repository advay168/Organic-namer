#pragma once

#include "Layout.h"

class VertexBuffer
{
private:
  unsigned int ID;
  Layout& layout;

public:
  VertexBuffer(Layout& layout, void* data, size_t size);
  void Bind();
  void setAttrib();
};