#include "Bond.h"

#include "Renderer/Renderer.h"

Bond::Bond(Atom* a, Atom* b)
  : a(a)
  , b(b)
{
  a->bonds.push_back(this);
  b->bonds.push_back(this);
}

Atom& Bond::other(Atom& x)
{
  return other(&x);
}
Atom& Bond::other(Atom* x)
{
  if (x == a)
    return *b;
  return *a;
}

void Bond::draw()
{
  Renderer::Line(a->pos, b->pos, 3.0f);
}
