#include "PhysicsFormatter.h"
#include <Renderer/Renderer.h>
#include <glm/trigonometric.hpp>

PhysicsFormatter::PhysicsFormatter(std::list<Atom>& atoms,
                                   std::list<Bond>& bonds)
  : atoms(atoms)
  , bonds(bonds)
{
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
    float dist = glm::distance(idealPositions[i], positions[i]);
    difference += dist * dist;
  }
  return { idealPositions, difference };
}

std::vector<glm::vec2> findOptimumArrangement(
  const std::vector<glm::vec2>& positions)
{
  float minVal = INFINITY;
  std::vector<glm::vec2> minPositions;
  for (float theta = 0.0f; theta < 7.0f; theta += 0.005f) {
    auto [newPositions, val] = getDifference(theta, positions);
    if (val < minVal) {
      minVal = val;
      minPositions = newPositions;
    }
  }
  return minPositions;
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
  // Set all forces to 0
  for (Atom& atom : atoms) {
    atom.force *= 0.0f;
  }
  for (Atom& centralAtom : atoms) {
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
