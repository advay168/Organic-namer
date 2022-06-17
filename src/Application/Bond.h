#pragma once
#include "Common.h"

#include "Application/Atom.h"

class Bond
{
public:
  Bond(Atom& a, Atom& b);

  void update();

  void draw();

private:
  Atom &a, b;
};
