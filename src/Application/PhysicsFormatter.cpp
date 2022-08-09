#include "PhysicsFormatter.h"
#include <Renderer/Renderer.h>
#include <glm/trigonometric.hpp>

PhysicsFormatter::PhysicsFormatter(Scene& scene)
  : scene(scene)
{
  for (uint8_t n = 0; n < MAX_BONDS; n++) {
    for (float theta = 0.0f; theta < TWO_PI; theta += 0.005f) {
      precomputedIdealPositions[n].push_back(calculateIdealPositions(theta, n));
    }
  }
}

PhysicsFormatter::PositionsArray_t PhysicsFormatter::calculateIdealPositions(
  float theta,
  uint8_t n)
{
  PositionsArray_t positions;
  float offset = TWO_PI / n;
  for (uint8_t i = 0; i < n; i++) {
    positions[i] = { glm::cos(theta + i * offset),
                     glm::sin(theta + i * offset) };
  }
  return positions;
}

float PhysicsFormatter::calculateDifference(
  const PositionsArray_t& idealPositions,
  const PositionsArray_t& positions,
  uint8_t n)
{
  // TODO: use permutations
  float difference = 0;
  for (uint8_t i = 0; i < n; i++) {
    float dist = glm::distance(idealPositions[i], positions[i]);
    difference += dist * dist;
  }
  return difference;
}

PhysicsFormatter::PositionsArray_t PhysicsFormatter::findOptimumArrangement(
  const PositionsArray_t& positions,
  uint8_t n)
{
  float minVal = INFINITY;
  PositionsArray_t minPositions;
  for (auto& idealPositions : precomputedIdealPositions[n]) {
    float val = calculateDifference(idealPositions, positions, n);
    if (val < minVal) {
      minVal = val;
      minPositions = idealPositions;
    }
  }
  return minPositions;
}

void PhysicsFormatter::applyForce()
{
  exertForce();
  for (Atom& atom : scene.atoms) {
    atom.pos += atom.force / 100.0f;
  }
}

void PhysicsFormatter::exertForce()
{
  // Set all forces to 0
  for (Atom& atom : scene.atoms) {
    atom.force *= 0.0f;
  }
  for (Atom& centralAtom : scene.atoms) {
    glm::vec2 centralPos = centralAtom.pos;
    PositionsArray_t positions;
    uint8_t n = 0;
    std::array<Atom*, MAX_BONDS> atoms;
    for (Bond* bond : centralAtom.bonds) {
      Atom& atom = bond->other(centralAtom);
      positions[n] = glm::normalize(atom.pos - centralPos);
      atoms[n] = &atom;
      n++;
    }
    PositionsArray_t newOffsets = findOptimumArrangement(positions, n);
    for (uint8_t i = 0; i < n; i++) {
      Atom& atom = *atoms[i];
      glm::vec2 newOffset = newOffsets[i];
      glm::vec2 newPos = BOND_LENGTH * newOffset + centralPos;
      glm::vec2 delta = newPos - atom.pos;
      atom.force += delta;
      centralAtom.force -= delta;
    }
  }
  optimiseForce();
}

void PhysicsFormatter::optimiseForce()
{
  if (scene.atoms.empty())
    return;
  glm::vec2 minForce(scene.atoms.begin()->force);
  glm::vec2 maxForce(scene.atoms.begin()->force);
  for (Atom& atom : scene.atoms) {
    minForce = glm::min(minForce, atom.force);
    maxForce = glm::max(maxForce, atom.force);
  }
  if (minForce.x > 0) {
    for (Atom& atom : scene.atoms) {
      atom.force.x -= minForce.x;
    }
  } else if (maxForce.x < 0) {
    for (Atom& atom : scene.atoms) {
      atom.force.x -= maxForce.x;
    }
  }
  if (minForce.y > 0) {
    for (Atom& atom : scene.atoms) {
      atom.force.y -= minForce.y;
    }
  } else if (maxForce.y < 0) {
    for (Atom& atom : scene.atoms) {
      atom.force.y -= maxForce.y;
    }
  }
}
