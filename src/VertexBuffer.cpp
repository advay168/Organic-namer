#include "VertexBuffer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

static unsigned int
sizeOf(unsigned int gl_type)
{
  switch (gl_type) {
    case GL_FLOAT:
      return sizeof(float);
    case GL_INT:
      return sizeof(int);
    case GL_UNSIGNED_INT:
      return sizeof(unsigned int);
  }
}

VertexBuffer::VertexBuffer(Layout& layout, void* data, size_t size)
  : layout(layout)
{
  glGenBuffers(1, &ID);
  Bind();

  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  setAttrib();
}

void
VertexBuffer::Bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void
VertexBuffer::setAttrib()
{
  unsigned int stride = 0;
  for (int i = 0; i < layout.types.size(); i++) {
    stride += sizeOf(layout.types[i]) * layout.counts[i];
  }
  unsigned int cumalativeSize = 0;
  for (int i = 0; i < layout.types.size(); i++) {
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i,
                          layout.counts[i],
                          layout.types[i],
                          GL_FALSE,
                          stride,
                          (void*)cumalativeSize);
    cumalativeSize += sizeOf(layout.types[i]) * layout.counts[i];
  }
}