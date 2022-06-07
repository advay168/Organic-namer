#pragma once

#include <vector>

class Layout
{
public:
  Layout& add(unsigned int gl_type, unsigned int count);
  Layout& add(unsigned int gl_type, unsigned int count, const char*)
  {
    return add(gl_type, count);
  }

  std::vector<unsigned int> types;
  std::vector<unsigned int> counts;
};