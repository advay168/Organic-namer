#pragma once
#include "Common.h"

#include "Atom.h"
#include "Bond.h"

class PhysicsFormatter
{
public:
  PhysicsFormatter(std::list<Atom>& atoms, std::list<Bond>& bonds);

  void exertForce();
  void applyForce();

  void draw();

private:
  void optimiseForce();

private:
  std::list<Atom>& atoms;
  std::list<Bond>& bonds;
  static constexpr float bondLength = 340.0f;
};
