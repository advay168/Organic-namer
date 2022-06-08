#include "Layout.h"

Layout& Layout::add(unsigned int gl_type, unsigned int count)
{
  types.push_back(gl_type);
  counts.push_back(count);
  return *this;
}