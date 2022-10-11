#pragma once
#include "Common.h"
#include "Config.h"

#include "Scene.h"

class PhysicsFormatter
{
public:
  PhysicsFormatter(Scene& scene);

  void exertForce();
  bool applyForce();

  void draw();

  void reArrangeBonds();

private:
  using PositionsArray_t = std::array<glm::vec2, MAX_BONDS>;

  PositionsArray_t calculateIdealPositions(float theta, uint8_t n);

  float calculateDifference(const PositionsArray_t& idealPositions,
                            const PositionsArray_t& positions,
                            uint8_t n);

  PositionsArray_t& findOptimumArrangement(const PositionsArray_t& positions,
                                           uint8_t n);

  void optimiseForce();

private:
  Scene& scene;

  std::array<std::vector<PositionsArray_t>, MAX_BONDS>
    precomputedIdealPositions;
};
