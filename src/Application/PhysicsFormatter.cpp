#include "PhysicsFormatter.h"
#include "glm/gtx/rotate_vector.hpp"
#include <Renderer/Renderer.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>

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

  if (glm::any(glm::isnan(bisector)))
    return;
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
  centralAtom.force -= deltaA + deltaB;
  // drawableAtoms.push_back({ newPosA, atomA.color, atomA.symbol });
  // drawableAtoms.push_back({ newPosB, atomB.color, atomB.symbol });
}

float func(float theta,
           const glm::vec2& v1,
           const glm::vec2& v2,
           const glm::vec2& v3)
{
  float deg120 = glm::pi<float>() * 2.0f / 3.0f;
  glm::vec2 offset1(glm::cos(theta + 0 * deg120), glm::sin(theta + 0 * deg120));
  glm::vec2 offset2(glm::cos(theta + 1 * deg120), glm::sin(theta + 1 * deg120));
  glm::vec2 offset3(glm::cos(theta + 2 * deg120), glm::sin(theta + 2 * deg120));

  float d1 = glm::distance(offset1, v1);
  float d2 = glm::distance(offset1, v2);
  float d3 = glm::distance(offset1, v3);

  return d1 + d2 + d3;
}

float optimiseTheta(const glm::vec2& v1,
                    const glm::vec2& v2,
                    const glm::vec2& v3)
{
  float minVal = INFINITY;
  float minTheta = 0.0f;
  for (float theta = 0.0f; theta < 10.0f; theta += 0.008f) {
    float val = func(theta, v1, v2, v3);
    if (val < minVal) {
      minVal = val;
      minTheta = theta;
    }
  }
  return minTheta;
}

void PhysicsFormatter::threeBonds(Atom& centralAtom)
{
  glm::vec2 centralPos = centralAtom.pos;

  Atom& atomA = centralAtom.bonds[0]->other(centralAtom);
  glm::vec2 posA = atomA.pos;
  glm::vec2 displacementA = posA - centralPos;

  Atom& atomB = centralAtom.bonds[1]->other(centralAtom);
  glm::vec2 posB = atomB.pos;
  glm::vec2 displacementB = posB - centralPos;

  Atom& atomC = centralAtom.bonds[2]->other(centralAtom);
  glm::vec2 posC = atomC.pos;
  glm::vec2 displacementC = posC - centralPos;

  float theta = optimiseTheta(glm::normalize(displacementA),
                              glm::normalize(displacementB),
                              glm::normalize(displacementC));
  float deg120 = glm::pi<float>() * 2.0f / 3.0f;
  glm::vec2 offset1(glm::cos(theta + 0 * deg120), glm::sin(theta + 0 * deg120));
  glm::vec2 offset2(glm::cos(theta + 1 * deg120), glm::sin(theta + 1 * deg120));
  glm::vec2 offset3(glm::cos(theta + 2 * deg120), glm::sin(theta + 2 * deg120));

  glm::vec2 newPosA = centralPos + bondLength * offset1;
  glm::vec2 newPosB = centralPos + bondLength * offset2;
  glm::vec2 newPosC = centralPos + bondLength * offset3;

  glm::vec2 deltaA = newPosA - posA;
  glm::vec2 deltaB = newPosB - posB;
  glm::vec2 deltaC = newPosC - posC;
  atomA.force += deltaA;
  atomB.force += deltaB;
  atomC.force += deltaC;
  centralAtom.force -= deltaA + deltaB + deltaC;

  drawableAtoms.clear();
  drawableAtoms.push_back({ newPosA, atomA.color, atomA.symbol });
  drawableAtoms.push_back({ newPosB, atomB.color, atomB.symbol });
  drawableAtoms.push_back({ newPosC, atomC.color, atomC.symbol });
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
  optimiseForce();
}

void PhysicsFormatter::optimiseForce()
{
  if (atoms.empty())
    return;
  glm::vec2 minForce(atoms.begin()->force);
  glm::vec2 maxForce(atoms.begin()->force);
  for (Atom& atom : atoms) {
    minForce = glm::min(minForce, atom.force);
    maxForce = glm::max(maxForce, atom.force);
  }
  if (minForce.x > 0) {
    for (Atom& atom : atoms) {
      atom.force.x -= minForce.x;
    }
  } else if (maxForce.x < 0) {
    for (Atom& atom : atoms) {
      atom.force.x -= maxForce.x;
    }
  }
  if (minForce.y > 0) {
    for (Atom& atom : atoms) {
      atom.force.y -= minForce.y;
    }
  } else if (maxForce.y < 0) {
    for (Atom& atom : atoms) {
      atom.force.y -= maxForce.y;
    }
  }
}
