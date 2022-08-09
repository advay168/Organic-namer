#include "PhysicsFormatter.h"
#include <Renderer/Renderer.h>
#include <glm/trigonometric.hpp>

PhysicsFormatter::PhysicsFormatter(Scene& scene)
  : scene(scene)
{
  for (size_t n = 0; n < MAX_BONDS; n++) {
    for (float theta = 0.0f; theta < 7.0f; theta += 0.005f) {
      precomputedIdealPositions[n].push_back(getIdealPositions(theta, n));
    }
  }
}

PhysicsFormatter::PrecomputedArray_t PhysicsFormatter::getIdealPositions(
  float theta,
  uint8_t n)
{
  PrecomputedArray_t positions;
  float offset = 2.0f * glm::pi<float>() / n;
  for (uint8_t i = 0; i < n; i++) {
    positions[i] = { glm::cos(theta + i * offset),
                     glm::sin(theta + i * offset) };
  }
  return positions;
}

float PhysicsFormatter::getDifference(const PrecomputedArray_t& idealPositions,
                                      const PrecomputedArray_t& positions, size_t n)
{
  // TODO: use permutations
  float difference = 0;
  for (uint8_t i = 0; i < n; i++) {
    float dist = glm::distance(idealPositions[i], positions[i]);
    difference += dist * dist;
  }
  return difference;
}

PhysicsFormatter::PrecomputedArray_t PhysicsFormatter::findOptimumArrangement(
  const PrecomputedArray_t& positions,
  size_t n)
{
  float minVal = INFINITY;
  PrecomputedArray_t minPositions;
  for (auto& idealPositions : precomputedIdealPositions[n]) {
    float val = getDifference(idealPositions, positions, n);
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
  for (Atom& atom : scene.atoms) {
    atom.force *= 0.0f;
  }
  for (Atom& centralAtom : scene.atoms) {
    glm::vec2 centralPos = centralAtom.pos;
    PrecomputedArray_t positions;
    size_t n = 0;
    std::vector<Atom*> atoms;
    for (Bond* bond : centralAtom.bonds) {
      Atom& atom = bond->other(centralAtom);
      positions[n] = glm::normalize(atom.pos - centralPos);
      atoms.push_back(&atom);
      n++;
    }
    PrecomputedArray_t newOffsets = findOptimumArrangement(positions, n);
    for (uint8_t i = 0; i < n; i++) {
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
