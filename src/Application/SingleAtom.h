#pragma once
#include "Common.h"

#include "Element.h"
#include "Atom.h"

struct SingleAtom
{
    ElementType element;

    const Atom* correspondent = nullptr;

    bool visited = false;

    struct BondedAtom
    {
        SingleAtom* bondedAtom;
        bool isSameBond = false;
    };

    std::vector<BondedAtom> bondedAtoms = {};

    inline bool isCarbon() { return element == ElementType::Carbon; }
    inline bool isNitrogen() { return element == ElementType::Nitrogen; }
    inline bool isOxygen() { return element == ElementType::Oxygen; }
    inline bool isHydrogen() { return element == ElementType::Hydrogen; }

    std::vector<SingleAtom*> getSingleBonds();
    std::vector<SingleAtom*> getDoubleBonds();
    std::vector<SingleAtom*> getTripleBonds();

    std::vector<SingleAtom*> findAtoms(ElementType::ElementTypeEnum el);

    std::vector<SingleAtom*> getUniqueBonds();

    void remove(SingleAtom* toRemove);
};
