#pragma once
#include "Common.h"

#include <unordered_map>

#include "Element.h"
#include "Scene.h"
#include "SingleAtom.h"
#include "FunctionalGroup.h"

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

    struct BrokenSubstituents
    {
        FunctionalGroup group;
        std::vector<std::pair<SingleAtom*, int>> substituents;
    };

    BrokenSubstituents findAndBreakHighestPriorityGroup(const std::vector<SingleAtom*>& chain);
    static bool contains(const std::vector<SingleAtom*>& atoms, ElementType::ElementTypeEnum el);
    static bool contains(const std::vector<SingleAtom*>& atoms, SingleAtom* atom);

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

    std::string nameOrganic(std::vector<SingleAtom*>& chain);
    std::string nameInorganic();


private:
    std::unordered_map<const Atom*, SingleAtom*> atomMapping;
    std::vector<std::unique_ptr<SingleAtom>> atomsList;
};
