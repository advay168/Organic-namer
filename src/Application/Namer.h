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

        template <ElementType::ElementTypeEnum el>
        std::vector<SingleAtom*> findAtoms()
        {
            std::vector<SingleAtom*> ret;
            for (auto& ba : bondedAtoms)
            {
                if (ba.bondedAtom->element == el && ba.isSameBond == false)
                    ret.push_back(ba.bondedAtom);
            }
            return ret;
        }

        std::vector<BondedAtom> getUniqueBonds();
    };

    enum class FunctionalGroup : uint8_t
    {
        CARBOXYLIC_ACID = 56,
        ESTER,
        AMIDE,
        NITRILE,
        ALDEHYDE,
        KETONE,
        ALCOHOL,
        AMINE,
        ALKYNE,
        ALKENE,
        ALKANE
    };

    void sortBonds();
    SingleAtom* findCarbonAtom();
    SingleAtom* findTerminalCarbonAtom(SingleAtom* carbonAtom);
    std::vector<SingleAtom*> findMaxCarbonChain(SingleAtom* carbonAtom);
    FunctionalGroup findHighestPriorityGroup(const std::vector<SingleAtom*>& chain);

    std::string nameOrganic(SingleAtom* carbonAtom);
    std::string nameInorganic();

    std::string namePrefix(int n);
    std::string namePrefix(FunctionalGroup group);
    std::string nameSuffix(FunctionalGroup group);
    std::string join(const std::string& a, const std::string& b);


private:
    std::unordered_map<const Atom*, SingleAtom*> atomMapping;
    std::vector<std::unique_ptr<SingleAtom>> atomsList;
};
