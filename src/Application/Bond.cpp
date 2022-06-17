#include "Bond.h"

#include "Renderer/Renderer.h"

Bond::Bond(Atom& a, Atom& b)
  : a(a)
  , b(b)
{
}

void Bond::update()
{
  void;
}

void Bond::draw()
{
  Renderer::Line(a.pos, b.pos, 3.0f);
}
