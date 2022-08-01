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
}

std::vector<glm::vec2> getIdealPositions(float theta, uint8_t n)
{
  std::vector<glm::vec2> positions;
  float offset = 2.0f * glm::pi<float>() / n;
  for (uint8_t i = 0; i < n; i++) {
    positions.push_back(
      { glm::cos(theta + i * offset), glm::sin(theta + i * offset) });
  }
  return positions;
}

struct Result
{
  std::vector<glm::vec2> newPositions;
  float difference;
};

Result getDifference(float theta, const std::vector<glm::vec2>& positions)
{
  // TODO: use permutations
  std::vector<glm::vec2> idealPositions =
    getIdealPositions(theta, positions.size());
  float difference = 0;
  for (uint8_t i = 0; i < positions.size(); i++) {
    difference += glm::distance(idealPositions[i], positions[i]);
  }
  return { idealPositions, difference };
}

std::vector<glm::vec2> findOptimumArrangement(
  const std::vector<glm::vec2>& positions)
{
  float minVal = INFINITY;
  std::vector<glm::vec2> minPositions;
  for (float theta = 0.0f; theta < 10.0f; theta += 0.008f) {
    auto [newPositions, val] = getDifference(theta, positions);
    if (val < minVal) {
      minVal = val;
      minPositions = newPositions;
    }
  }
  return minPositions;
}

void PhysicsFormatter::draw()
{
  for (auto& [pos, color, symbol] : drawableAtoms)
    Renderer::TextCircle(pos, 40.0f, color, symbol, 1.0f, glm::vec3(1.0f));
  drawableAtoms.clear();
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
    if(centralAtom.bonds.size() <= 1)
      continue;
    glm::vec2 centralPos = centralAtom.pos;
    std::vector<glm::vec2> positions;
    std::vector<Atom*> atoms;
    for (Bond* bond : centralAtom.bonds) {
      Atom& atom = bond->other(centralAtom);
      if (&atom != &centralAtom) {
        positions.push_back(glm::normalize(atom.pos - centralPos));
        atoms.push_back(&atom);
      }
    }
    std::vector<glm::vec2> newOffsets = findOptimumArrangement(positions);
    for (uint8_t i = 0; i < newOffsets.size(); i++) {
      Atom& atom = *atoms[i];
      glm::vec2 newOffset = newOffsets[i];
      glm::vec2 newPos = bondLength * newOffset + centralPos;
      //drawableAtoms.push_back({newPos, atom.color, atom.symbol});
      glm::vec2 delta = newPos - atom.pos;
      atom.force += delta;
      centralAtom.force -= delta;
    }
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
