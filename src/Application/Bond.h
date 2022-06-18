#pragma once
#include "Common.h"

#include "Application/Atom.h"

struct Bond
{
  Bond(Atom* a = nullptr, Atom* b = nullptr);

  void draw();

  Atom& other(Atom& x);
  Atom& other(Atom* x);

  Atom *a, *b;
};
