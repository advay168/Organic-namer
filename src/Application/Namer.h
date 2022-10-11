#pragma once
#include "Common.h"

#include <unordered_map>

#include "Element.h"
#include "FunctionalGroup.h"
#include "Scene.h"
#include "SingleAtom.h"

class Namer
{
public:
    Namer(const Scene& scene);
    std::string getName();

private:
    void sortBonds();
    bool isConnected();

    std::vector<SingleAtom*> findCarbonAtoms();
    SingleAtom* findTerminalCarbonAtom(SingleAtom* carbonAtom);
    std::vector<SingleAtom*> findMaxCarbonChain(SingleAtom* carbonAtom);

    SingleAtom* findAtom(const std::vector<SingleAtom*>& atoms, ElementType::ElementTypeEnum el);

    struct BrokenSubstituents
    {
        FunctionalGroup group;
        std::vector<std::pair<SingleAtom*, int>> substituents;
    };

    std::pair<int, BrokenSubstituents> findAndBreakHighestPriorityGroup(const std::vector<SingleAtom*>& chain);
    static bool contains(const std::vector<SingleAtom*>& atoms, ElementType::ElementTypeEnum el);
    static bool contains(const std::vector<SingleAtom*>& atoms, SingleAtom* atom);

    std::pair<int, BrokenSubstituents> findCARBOXYLIC_ACID(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findESTER(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findAMIDE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findNITRILE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findALDEHYDE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findKETONE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findALCOHOL(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findAMINE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findALKYNE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findALKENE(const std::vector<SingleAtom*>& chain);
    std::pair<int, BrokenSubstituents> findALKANE(const std::vector<SingleAtom*>& chain);

    std::string nameOrganic(std::vector<SingleAtom*>& chain);
    std::string nameInorganic();

private:
    std::unordered_map<const Atom*, SingleAtom*> atomMapping;
    std::vector<std::unique_ptr<SingleAtom>> atomsList;
    SingleAtom sentinelHydrogen { ElementType::Hydrogen };
};
