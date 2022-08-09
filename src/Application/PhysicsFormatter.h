#pragma once
#include "Common.h"

#include <array>

#include "Scene.h"

class PhysicsFormatter
{
public:
  PhysicsFormatter(Scene& scene);

  void exertForce();
  void applyForce();

  void draw();

private:
  static constexpr size_t MAX_BONDS = 10;
  using PrecomputedArray_t = std::array<glm::vec2, MAX_BONDS>;

  PrecomputedArray_t getIdealPositions(float theta, uint8_t n);

  float getDifference(const PrecomputedArray_t& idealPositions,
                       const PrecomputedArray_t& positions, size_t n);

  PrecomputedArray_t findOptimumArrangement(
    const PrecomputedArray_t& positions, size_t n);

  void optimiseForce();

private:
  Scene& scene;
  static constexpr float bondLength = 340.0f;

  std::array<std::vector<PrecomputedArray_t>, MAX_BONDS>
    precomputedIdealPositions;
};
