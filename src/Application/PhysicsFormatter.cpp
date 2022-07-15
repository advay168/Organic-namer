#include "PhysicsFormatter.h"
#include "glm/gtx/rotate_vector.hpp"
#include <Renderer/Renderer.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>

PhysicsFormatter::PhysicsFormatter(std::list<Atom>& atoms,
                                   std::list<Bond>& bonds)
  : atoms(atoms)
  , bonds(bonds)
{
}

void PhysicsFormatter::twoBonds(Atom& centralAtom)
{
  glm::vec2 centralPos = centralAtom.pos;
  Atom& atomA = centralAtom.bonds[0]->other(centralAtom);
  glm::vec2 posA = atomA.pos;
  glm::vec2 displacementA = posA - centralPos;

  Atom& atomB = centralAtom.bonds[1]->other(centralAtom);
  glm::vec2 posB = atomB.pos;
  glm::vec2 displacementB = posB - centralPos;

  glm::vec2 bisector = glm::normalize(glm::normalize(displacementA) +
                                      glm::normalize(displacementB)) *
                       bondLength;

  float deg90 = glm::pi<float>() / 2.0f;
  glm::vec2 newPos1 = centralPos + glm::rotate(bisector, deg90);
  glm::vec2 newPos2 = centralPos + glm::rotate(bisector, -deg90);
  glm::vec2 newPosA, newPosB;
  if (glm::distance(newPos1, posA) < glm::distance(newPos2, posA)) {
    newPosA = newPos1;
    newPosB = newPos2;
  } else {
    newPosA = newPos2;
    newPosB = newPos1;
  }
  glm::vec2 deltaA = newPosA - posA;
  glm::vec2 deltaB = newPosB - posB;
  atomA.force += deltaA;
  atomB.force += deltaB;
  // drawableAtoms.push_back({ newPosA, atomA.color, atomA.symbol });
  // drawableAtoms.push_back({ newPosB, atomB.color, atomB.symbol });
}

void PhysicsFormatter::threeBonds(Atom& centralAtom)
{
  (void)centralAtom;
}

void PhysicsFormatter::fourBonds(Atom& centralAtom)
{
  (void)centralAtom;
}

void PhysicsFormatter::draw()
{
  for (auto& [pos, color, symbol] : drawableAtoms)
    Renderer::TextCircle(pos, 40.0f, color, symbol, 1.0f, glm::vec3(1.0f));
}

void PhysicsFormatter::applyForce()
{
  exertForce();
  for (Atom& atom : atoms) {
    if (glm::length(atom.force) > 0) {
      glm::vec2 forceDir = glm::normalize(atom.force);
      float forceMag = glm::length(atom.force);
      atom.pos += forceDir * forceMag / 100.0f;
    }
  }
}

void PhysicsFormatter::exertForce()
{
  for (Atom& atom : atoms) {
    atom.force *= 0.0f;
  }
  for (Atom& centralAtom : atoms) {
    int numBonds = centralAtom.bonds.size();
    switch (numBonds) {
      case 2:
        twoBonds(centralAtom);
        break;
      case 3:
        threeBonds(centralAtom);
        break;
      case 4:
        fourBonds(centralAtom);
        break;
      case 1:
      default:
        break;
    }
    // for (Bond* bond1 : centralAtom.bonds) {
    // Atom& atom1 = bond1->other(centralAtom);
    // glm::vec2 bond1Dir = glm::normalize(atom1.pos - centralAtom.pos);
    // glm::vec2 hopefulPosition1 = centralAtom.pos + bond1Dir * 30.0f;
    // glm::vec2 bond1Center = (centralAtom.pos + hopefulPosition1) / 2.0f;
    // for (Bond* bond2 : centralAtom.bonds) {
    // if (bond1 == bond2)
    // continue;
    // Atom& atom2 = bond2->other(centralAtom);
    // glm::vec2 bond2Dir = glm::normalize(atom2.pos - centralAtom.pos);
    // glm::vec2 hopefulPosition2 = centralAtom.pos + bond2Dir * 30.0f;
    // glm::vec2 bond2Center = (centralAtom.pos + hopefulPosition2) / 2.0f;
    // glm::vec2 diff = bond2Center - bond1Center;
    // float dist = diff.length();
    // float forceMag = 1.0f / (dist * dist);
    // glm::vec2 dir = -diff / dist;
    // glm::vec2 forceOnBond1 = forceMag * dir;
    // glm::vec2 forceOnBond2 = -forceOnBond1;
    // atom1.force += forceOnBond1 / 2.0f;
    // atom2.force += forceOnBond2 / 2.0f;
    //}
    //}
  }
}
