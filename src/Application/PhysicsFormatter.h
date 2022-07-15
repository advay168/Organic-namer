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
  void twoBonds(Atom& centralAtom);
  void threeBonds(Atom& centralAtom);
  void fourBonds(Atom& centralAtom);

private:
  std::list<Atom>& atoms;
  std::list<Bond>& bonds;
  static constexpr float bondLength = 240.0f;

  struct DrawableAtom
  {
    glm::vec2 pos;
    glm::vec3 color;
    std::string symbol;
  };
  std::vector<DrawableAtom> drawableAtoms;
};
