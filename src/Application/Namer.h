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

        std::vector<SingleAtom*> getSingleBonds();
        std::vector<SingleAtom*> getDoubleBonds();
        std::vector<SingleAtom*> getTripleBonds();

        std::vector<SingleAtom*> findAtoms(ElementType::ElementTypeEnum el);

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
    bool isConnected();

    SingleAtom* findCarbonAtom();
    SingleAtom* findTerminalCarbonAtom(SingleAtom* carbonAtom);
    std::vector<SingleAtom*> findMaxCarbonChain(SingleAtom* carbonAtom);

    struct BrokenSubstituents
    {
        FunctionalGroup group;
        std::vector<std::pair<SingleAtom*, int>> substituents;
    };

    BrokenSubstituents findAndBreakHighestPriorityGroup(const std::vector<SingleAtom*>& chain);
    bool contains(const std::vector<SingleAtom*> atoms, ElementType::ElementTypeEnum el);

    std::pair<bool, BrokenSubstituents> findCARBOXYLIC_ACID(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findESTER(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findAMIDE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findNITRILE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findALDEHYDE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findKETONE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findALCOHOL(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findAMINE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findALKYNE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findALKENE(const std::vector<SingleAtom*>& chain);
    std::pair<bool, BrokenSubstituents> findALKANE(const std::vector<SingleAtom*>& chain);

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
