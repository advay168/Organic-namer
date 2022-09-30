#pragma once
#include "Common.h"

#include <unordered_map>

#include "Element.h"
#include "Scene.h"

class Namer
{
public:
    Namer(const Scene& scene);
    std::string getName();

private:
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

        std::vector<std::tuple<BondedAtom>> getSingleBonds();
        std::vector<std::tuple<BondedAtom, BondedAtom>> getDoubleBonds();
        std::vector<std::tuple<BondedAtom, BondedAtom, BondedAtom>> getTripleBonds();

        std::vector<BondedAtom> getUniqueBonds();
    };

    void sortBonds();
    SingleAtom* findCarbonAtom();
    SingleAtom* findTerminalCarbonAtom(SingleAtom* carbonAtom);

    std::string nameOrganic(SingleAtom* carbonAtom);
    std::string nameInorganic();

private:
    std::unordered_map<const Atom*, SingleAtom*> atomMapping;
    std::vector<std::unique_ptr<SingleAtom>> atomsList;
};
