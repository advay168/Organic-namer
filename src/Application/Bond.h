#pragma once
#include "Common.h"

#include "Application/Atom.h"

struct Bond
{
  Bond(Atom* a = nullptr, Atom* b = nullptr);

  Bond(const Bond& other);
  ~Bond();

  Bond(Bond&&) = delete;
  Bond& operator=(Bond&) = delete;
  Bond& operator=(Bond&&) = delete;

  bool operator==(Bond& other);

  void draw();

  Atom& other(Atom& x);
  Atom& other(Atom* x);

  float length();

  Atom *a, *b;

  uint8_t count = 1;
};
