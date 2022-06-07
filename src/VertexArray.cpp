#include "VertexArray.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &ID);
  Bind();
}

void
VertexArray::Bind()
{
  glBindVertexArray(ID);
}