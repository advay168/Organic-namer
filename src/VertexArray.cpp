#include "VertexArray.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &ID);
  Bind();
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &ID);
}

void VertexArray::Bind()
{
  glBindVertexArray(ID);
}