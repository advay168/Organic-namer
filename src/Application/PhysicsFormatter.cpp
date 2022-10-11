#include "PhysicsFormatter.h"
#include <glm/trigonometric.hpp>

#include <Renderer/Renderer.h>

PhysicsFormatter::PhysicsFormatter(Scene& scene)
    : scene(scene)
{
    auto comparer = [](const glm::vec2& a, const glm::vec2& b)
    {
        // return a < b
        return glm::acos(a.x) < glm::acos(b.x);
    };
    for (uint8_t n = 1; n < MAX_BONDS; n++)
    {
        for (float theta = 0.0f; theta < TWO_PI; theta += 0.005f)
        {
            auto temp = calculateIdealPositions(theta, n);
#if (false)
            std::sort(temp.begin(), temp.end(), comparer);
            do
            {
                precomputedIdealPositions[n].push_back(temp);
            } while (std::next_permutation(
                temp.begin() + 1,
                temp.end(),
                comparer)); // Rotation takes care of first element
#else
            (void)comparer;
            precomputedIdealPositions[n].push_back(temp);
#endif
        }
    }
}

PhysicsFormatter::PositionsArray_t PhysicsFormatter::calculateIdealPositions(
    float theta,
    uint8_t n)
{
    PositionsArray_t positions;
    float offset = TWO_PI / n;
    for (uint8_t i = 0; i < n; i++)
    {
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
    float difference = 0;
    for (uint8_t i = 0; i < n; i++)
    {
        float dist = glm::distance(idealPositions[i], positions[i]);
        difference += dist * dist;
    }
    return difference;
}

PhysicsFormatter::PositionsArray_t& PhysicsFormatter::findOptimumArrangement(
    const PositionsArray_t& positions,
    uint8_t n)
{
    float minVal = INFINITY;
    PositionsArray_t* minPositionsPtr;
    for (auto& idealPositions : precomputedIdealPositions[n])
    {
        float val = calculateDifference(idealPositions, positions, n);
        if (val < minVal)
        {
            minVal = val;
            minPositionsPtr = &idealPositions;
        }
    }
    return *minPositionsPtr;
}

bool PhysicsFormatter::applyForce()
{
    glm::vec2 delta(0.0f);
    exertForce();
    for (Atom& atom : scene.atoms)
    {
        delta += glm::abs(atom.force);
        atom.pos += atom.force * simulationSpeed;
    }
    deltaLength = glm::length(delta);
    return deltaLength < 2.0f;
}

void PhysicsFormatter::exertForce()
{
    // Set all forces to 0
    for (Atom& atom : scene.atoms)
    {
        atom.force *= 0.0f;
    }
    for (Atom& centralAtom : scene.atoms)
    {
        glm::vec2 centralPos = centralAtom.pos;
        PositionsArray_t positions;
        uint8_t n = 0;
        std::array<Atom*, MAX_BONDS> atoms;
        for (Bond* bond : centralAtom.bonds)
        {
            Atom& atom = bond->other(centralAtom);
            positions[n] = glm::normalize(atom.pos - centralPos);
            atoms[n] = &atom;
            n++;
        }
        PositionsArray_t newOffsets = findOptimumArrangement(positions, n);
        for (uint8_t i = 0; i < n; i++)
        {
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

void PhysicsFormatter::reArrangeBonds()
{
    for (Atom& atom : scene.atoms)
    {
        std::sort(atom.bonds.begin(), atom.bonds.end(),
                  [&atom](Bond* b1, Bond* b2)
                  {
                      Atom& a1 = b1->other(atom);
                      glm::vec2 diff = a1.pos - atom.pos;
                      float v1 = glm::atan(diff.y, diff.x);
                      Atom& a2 = b2->other(atom);
                      diff = a2.pos - atom.pos;
                      float v2 = glm::atan(diff.y, diff.x);
                      return v1 < v2;
                  });
    }
}

void PhysicsFormatter::optimiseForce()
{
    if (scene.atoms.empty())
        return;
    glm::vec2 minForce(scene.atoms.begin()->force);
    glm::vec2 maxForce(scene.atoms.begin()->force);
    for (Atom& atom : scene.atoms)
    {
        minForce = glm::min(minForce, atom.force);
        maxForce = glm::max(maxForce, atom.force);
    }
    if (minForce.x > 0)
    {
        for (Atom& atom : scene.atoms)
        {
            atom.force.x -= minForce.x;
        }
    }
    else if (maxForce.x < 0)
    {
        for (Atom& atom : scene.atoms)
        {
            atom.force.x -= maxForce.x;
        }
    }
    if (minForce.y > 0)
    {
        for (Atom& atom : scene.atoms)
        {
            atom.force.y -= minForce.y;
        }
    }
    else if (maxForce.y < 0)
    {
        for (Atom& atom : scene.atoms)
        {
            atom.force.y -= maxForce.y;
        }
    }
}
