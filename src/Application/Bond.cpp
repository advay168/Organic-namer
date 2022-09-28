#include "Bond.h"

#include "Renderer/Renderer.h"

#include "glm/ext/scalar_constants.hpp"
#include "glm/gtx/rotate_vector.hpp"

Bond::Bond(Atom* a, Atom* b)
    : a(a)
    , b(b)
{
    if (a->bonds.size() >= MAX_BONDS - 1 || b->bonds.size() >= MAX_BONDS - 1)
    {
        throw std::runtime_error("Tried adding too many bonds");
    }
    a->bonds.push_back(this);
    b->bonds.push_back(this);
}

Bond::Bond(const Bond& other)
    : a(other.a)
    , b(other.b)
{
    a->bonds.push_back(this);
    b->bonds.push_back(this);
}

Bond::~Bond()
{
    auto& aBonds = a->bonds;
    auto& bBonds = b->bonds;
    bBonds.erase(std::find(bBonds.begin(), bBonds.end(), this));
    aBonds.erase(std::find(aBonds.begin(), aBonds.end(), this));
}

bool Bond::operator==(Bond& other)
{
    if (a == other.a && b == other.b)
        return true;
    return a == other.b && b == other.a;
}

Atom& Bond::other(Atom& x)
{
    return other(&x);
}
Atom& Bond::other(Atom* x)
{
    if (x == a)
        return *b;
    return *a;
}

float Bond::length()
{
    return (a->pos - b->pos).length();
}

void Bond::draw()
{
    glm::vec2 line(b->pos - a->pos);
    glm::vec2 offset(glm::normalize(glm::rotate(line, float(PI) / 2.0f)) * 5.0f);
    switch (count)
    {
    case 1:
    {
        Renderer::Line(a->pos, b->pos, 3.0f);
        return;
    }
    case 2:
    {
        Renderer::Line(a->pos + offset, b->pos + offset, 2.0f);
        Renderer::Line(a->pos - offset, b->pos - offset, 2.0f);
        return;
    }
    case 3:
    {
        Renderer::Line(a->pos, b->pos, 2.0f);
        Renderer::Line(a->pos + 1.5f * offset, b->pos + 1.5f * offset, 2.0f);
        Renderer::Line(a->pos - 1.5f * offset, b->pos - 1.5f * offset, 2.0f);
        return;
    }
    case 4:
    {
        Renderer::Line(a->pos + 2.5f * offset, b->pos + 2.5f * offset, 2.0f);
        Renderer::Line(a->pos + 0.75f * offset, b->pos + 0.75f * offset, 2.0f);
        Renderer::Line(a->pos - 0.75f * offset, b->pos - 0.75f * offset, 2.0f);
        Renderer::Line(a->pos - 2.5f * offset, b->pos - 2.5f * offset, 2.0f);
        return;
    }
    }
    Renderer::Line(a->pos, b->pos, 3.0f);
    Renderer::Text(std::to_string(count), (a->pos + b->pos) / 2.0f, 1.0f);
}
