#include "Namer.h"
#include <array>
#include <functional>
#include <glm/gtc/constants.hpp>

template <size_t n, typename T>
std::array<T, n> shatter(std::vector<T>& vec)
{
    std::array<T, n> arr;
    std::copy(arr.begin(), arr.begin() + n, vec.begin());
    return arr;
}

Namer::Namer(const Scene& scene)
{
    for (const Atom& atom : scene.atoms)
    {
        SingleAtom newAtom { atom.element, &atom };
        atomsList.push_back(std::make_unique<SingleAtom>(newAtom));
        atomMapping[&atom] = atomsList.back().get();
    }

    for (auto& [a, b, count] : scene.bonds)
    {
        auto atomA = atomMapping[a];
        auto atomB = atomMapping[b];
        atomA->bondedAtoms.push_back({ atomB, false });
        atomB->bondedAtoms.push_back({ atomA, false });
        for (uint8_t temp = count - 1; temp > 0; temp--)
        {
            atomA->bondedAtoms.push_back({ atomB, true });
            atomB->bondedAtoms.push_back({ atomA, true });
        }
    }
    for (auto& atom : atomsList)
    {
        if (atom->isHydrogen() && atom->bondedAtoms.size() != 1)
            throw std::runtime_error("Hydrogen atom does not have exactly 1 bond.");
        if (atom->isOxygen() && atom->bondedAtoms.size() != 2)
            throw std::runtime_error("Oxygen atom does not have exactly 2 bonds.");
        if (atom->isNitrogen() && atom->bondedAtoms.size() != 3)
            throw std::runtime_error("Nitrogen atom does not have exactly 3 bonds.");
        if (atom->isCarbon() && atom->bondedAtoms.size() != 4)
            throw std::runtime_error("Carbon atom does not have exactly 4 bonds.");
    }
    sortBonds();
    if (!isConnected())
        throw std::runtime_error("All atoms are not connected");
}

std::string Namer::getName()
{
    std::vector<SingleAtom*> carbonAtoms = findCarbonAtoms();
    if (carbonAtoms.empty())
        return nameInorganic();
    std::vector<SingleAtom*> maxPath;
    size_t maxLength = 0;
    for (SingleAtom* carbonAtom : carbonAtoms)
    {
        carbonAtom = findTerminalCarbonAtom(carbonAtom);
        auto path = findMaxCarbonChain(carbonAtom);
        if (path.size() > maxLength)
        {
            maxLength = path.size();
            maxPath = path;
        }
    }
    return nameOrganic(maxPath);
}

void Namer::sortBonds()
{
    for (auto& atom : atomsList)
    {
        auto copy = atom->bondedAtoms;
        atom->bondedAtoms.clear();
        for (ElementType el : { ElementType::Carbon, ElementType::Nitrogen, ElementType::Oxygen, ElementType::Hydrogen })
        {
            for (SingleAtom::BondedAtom ba : copy)
            {
                if (ba.bondedAtom->element == el)
                {
                    atom->bondedAtoms.push_back(ba);
                }
            }
        }
    }
}

bool Namer::isConnected()
{
    if (atomsList.empty())
        return true;
    for (auto& atomPtr : atomsList)
        atomPtr->visited = false;
    std::function<void(SingleAtom*)> helper = [&helper](SingleAtom* atom)
    {
        atom->visited = true;
        for (SingleAtom::BondedAtom ba : atom->bondedAtoms)
        {
            if (!ba.bondedAtom->visited)
                helper(ba.bondedAtom);
        }
    };
    SingleAtom* initial = atomsList[0].get();
    helper(initial);
    for (auto& atomPtr : atomsList)
    {
        if (!atomPtr->visited)
            return false;
    }
    return true;
}

std::vector<SingleAtom*> Namer::findCarbonAtoms()
{
    std::vector<SingleAtom*> atoms;
    for (auto& atom : atomsList)
    {
        if (atom->isCarbon())
            atoms.push_back(atom.get());
    }
    return atoms;
}

SingleAtom* Namer::findTerminalCarbonAtom(SingleAtom* carbonAtom)
{
    for (auto& atom : atomsList)
    {
        atom->visited = false;
    }
    std::function<SingleAtom*(SingleAtom*)> helper = [&helper](SingleAtom* carbonAtom)
    {
        carbonAtom->visited = true;
        for (auto ba : carbonAtom->bondedAtoms)
        {
            if (!ba.bondedAtom->visited && ba.bondedAtom->isCarbon())
                return helper(ba.bondedAtom);
        }
        return carbonAtom;
    };
    return helper(carbonAtom);
}

std::vector<SingleAtom*> Namer::findMaxCarbonChain(SingleAtom* carbonAtom)
{
    for (auto& atom : atomsList)
    {
        atom->visited = false;
    }
    using ret_t = std::vector<SingleAtom*>;
    std::function<ret_t(SingleAtom*)> helper = [&helper](SingleAtom* carbonAtom) -> ret_t
    {
        carbonAtom->visited = true;
        auto carbs = carbonAtom->findAtoms(ElementType::Carbon);
        uint32_t maxLength = 0;
        ret_t ret;
        for (auto atom : carbs)
        {
            if (!atom->visited)
            {
                auto path = helper(atom);
                if (path.size() > maxLength)
                {
                    maxLength = path.size();
                    ret = path;
                }
            }
        }
        ret.push_back(carbonAtom);
        return ret;
    };
    auto path = helper(carbonAtom);
    std::reverse(path.begin(), path.end());
    return path;
}

SingleAtom* Namer::findAtom(const std::vector<SingleAtom*>& atoms, ElementType::ElementTypeEnum el)
{
    for (auto& atom : atoms)
        if (atom->element == el)
            return atom;
    return nullptr;
}

std::pair<int, Namer::BrokenSubstituents> Namer::findAndBreakHighestPriorityGroup(const std::vector<SingleAtom*>& chain)
{

    for (SingleAtom* atom : chain)
    {
        assert(atom->isCarbon());
    }
    int idx;
    BrokenSubstituents ret;
    std::tie(idx, ret) = findCARBOXYLIC_ACID(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findESTER(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findAMIDE(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findNITRILE(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findALDEHYDE(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findKETONE(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findALCOHOL(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findAMINE(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findALKYNE(chain);
    if (idx != -1)
        return { idx, ret };
    std::tie(idx, ret) = findALKENE(chain);
    if (idx != -1)
        return { idx, ret };
    return findALKANE(chain);
}

bool Namer::contains(const std::vector<SingleAtom*>& atoms, ElementType::ElementTypeEnum el)
{
    for (auto atom : atoms)
    {
        if (atom->element == el)
            return true;
    }
    return false;
}

bool Namer::contains(const std::vector<SingleAtom*>& atoms, SingleAtom* atom)
{
    return std::find(
               atoms.begin(),
               atoms.end(),
               atom)
        != atoms.end();
}

std::pair<int, Namer::BrokenSubstituents> Namer::findCARBOXYLIC_ACID(const std::vector<SingleAtom*>& chain)
{
    SingleAtom* AcidicCarbon = nullptr;
    for (auto atom : chain)
    {
        std::vector<SingleAtom*> oxygens = atom->findAtoms(ElementType::Oxygen);
        if (oxygens.size() != 3)
            continue;
        auto [a, b, c] = shatter<3>(oxygens);
        if (a != b && b != c)
            continue;
        if (atom->bondedAtoms[3].bondedAtom->isHydrogen())
            AcidicCarbon = atom;
    }
    return { bool(AcidicCarbon) ? 1 : -1, { FunctionalGroup::CARBOXYLIC_ACID, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findESTER(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::ESTER, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findAMIDE(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::AMIDE, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findNITRILE(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::NITRILE, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findALDEHYDE(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::ALDEHYDE, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findKETONE(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::KETONE, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findALCOHOL(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::ALCOHOL, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findAMINE(const std::vector<SingleAtom*>& chain)
{
    (void)chain;
    return { -1, { FunctionalGroup::AMINE, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findALKYNE(const std::vector<SingleAtom*>& chain)
{
    SingleAtom* tripleCarbon = nullptr;
    for (auto atom : chain)
    {
        std::vector<SingleAtom*> triples = atom->getTripleBonds();
        if (contains(triples, ElementType::Carbon) && contains(chain, findAtom(triples, ElementType::Carbon)))
            tripleCarbon = atom;
    }
    return { tripleCarbon ? 1 : -1, { FunctionalGroup::ALKYNE, {} } };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findALKENE(const std::vector<SingleAtom*>& chain)
{
    int idx = -1;
    for (size_t i = 0; i < chain.size(); i++)
    {
        auto atom = chain[i];
        std::vector<SingleAtom*> doubles = atom->getDoubleBonds();
        if (contains(doubles, ElementType::Carbon) && contains(chain, findAtom(doubles, ElementType::Carbon)))
            idx = i;
    }
    BrokenSubstituents brokenSubstituents { FunctionalGroup::ALKENE, {} };
    if (idx == -1)
        return { -1, brokenSubstituents };
    size_t l = chain.size();
    int dir = 1;
    int i = 0;
    int end = chain.size();
    if (idx > (int)l - idx)
    {
        dir = -1;
        i = chain.size() - 1;
        end = -1;
        idx = chain.size() - idx - 1;
    }
    int pos = 0;
    for (; i * dir < end * dir; i += dir)
    {
        pos++;
        SingleAtom* atom = chain[i];
        std::vector<SingleAtom*> unique = atom->getUniqueBonds();
        for (auto uniqueAtom : unique)
        {
            if (contains(chain, uniqueAtom))
                continue;
            int count = uniqueAtom->remove(atom);
            for (int i = 0; i < count; i++)
                uniqueAtom->bondedAtoms.push_back({ &sentinelHydrogen, false });
            brokenSubstituents.substituents.push_back({ uniqueAtom, pos });
        }
    }
    return { idx + 1, brokenSubstituents };
}

std::pair<int, Namer::BrokenSubstituents> Namer::findALKANE(const std::vector<SingleAtom*>& chain)
{
    BrokenSubstituents brokenSubstituents { FunctionalGroup::ALKANE, {} };
    for (size_t i = 0; i < chain.size(); i++)
    {
        SingleAtom* atom = chain[i];
        std::vector<SingleAtom*> unique = atom->getUniqueBonds();
        for (auto uniqueAtom : unique)
        {
            if (contains(chain, uniqueAtom))
                continue;
            int count = uniqueAtom->remove(atom);
            for (int i = 0; i < count; i++)
                uniqueAtom->bondedAtoms.push_back({ &sentinelHydrogen, false });
            brokenSubstituents.substituents.push_back({ uniqueAtom, i + 1 });
        }
    }
    return { true, brokenSubstituents };
}

std::string Namer::nameOrganic(std::vector<SingleAtom*>& chain)
{
    for (SingleAtom* atom : chain)
        ((Atom*)atom->correspondent)->color = glm::vec3(1.0f);
    int size = chain.size();
    std::string prefix = namePrefix(size);
    auto [idx, subs] = findAndBreakHighestPriorityGroup(chain);
    auto [group, substituents] = subs;
    std::string name = join(prefix, nameSuffix(group, idx));
    for (auto& [headAtom, index] : substituents)
    {
        if (headAtom->isHydrogen())
            continue;
        if (headAtom->isCarbon())
        {
            std::vector<SingleAtom*> subChain = findMaxCarbonChain(headAtom);
            auto [subIdx, subSubs] = findAndBreakHighestPriorityGroup(subChain);
            auto [subGroup, subSubstituents] = subSubs;
            std::string n = namePrefix(subChain.size());
            name = "-" + std::to_string(index) + "-" + join(n, namePrefix(subGroup)) + name;
        }
    }
    if (name[0] == '-')
        name = name.substr(1);

    return name;
}

std::string Namer::nameInorganic()
{
    return "CO2";
}
