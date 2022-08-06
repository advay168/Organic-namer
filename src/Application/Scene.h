#pragma once
#include "Common.h"

#include <Application/Atom.h>
#include <Application/Bond.h>

struct Scene
{
  void deserialise(std::istream& stream);

  void serialize(std::ostream& stream);

  std::list<Atom> atoms;
  std::list<Bond> bonds;
};
