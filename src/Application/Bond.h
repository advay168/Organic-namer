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

  bool operator==(Bond& other) const;

  void draw();

  Atom& other(Atom& x) const;
  Atom& other(Atom* x) const;

  float length() const;

  Atom *a, *b;

  uint8_t count = 1;
};
