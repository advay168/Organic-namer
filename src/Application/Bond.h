#pragma once
#include "Common.h"

#include "Application/Atom.h"

class Bond
{
public:
  Bond(Atom* a = nullptr, Atom* b = nullptr);

  void draw();

  Atom& other(Atom& x);
  Atom& other(Atom* x);

private:
  Atom *a, *b;
};
