#pragma once
#include "Common.h"

#include "Layout.h"

class VertexBuffer
{
private:
  unsigned int ID;
  Layout& layout;

public:
  VertexBuffer(Layout& layout);
  VertexBuffer(Layout& layout, void* data, size_t size);

  void setData(void* data, size_t size);
  void subData(void* data, size_t offset, size_t size);

  void Bind();
  void setAttrib();
};
