#pragma once
#include "Common.h"

#include "Element.h"
#include "Scene.h"

class IR
{
};

class Namer
{
public:
    Namer(const Scene& scene);
    std::string getName();


private:
    IR ir;
};
