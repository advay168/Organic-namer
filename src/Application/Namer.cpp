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

Namer::BrokenSubstituents Namer::findAndBreakHighestPriorityGroup(const std::vector<SingleAtom*>& chain)
{

    for (SingleAtom* atom : chain)
    {
        assert(atom->isCarbon());
    }
    bool found;
    BrokenSubstituents ret;
    std::tie(found, ret) = findCARBOXYLIC_ACID(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findESTER(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findAMIDE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findNITRILE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findALDEHYDE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findKETONE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findALCOHOL(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findAMINE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findALKYNE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findALKENE(chain);
    if (found)
        return ret;
    std::tie(found, ret) = findALKANE(chain);
    return ret;
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

std::pair<bool, Namer::BrokenSubstituents> Namer::findCARBOXYLIC_ACID(const std::vector<SingleAtom*>& chain)
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
    return { bool(AcidicCarbon), { FunctionalGroup::CARBOXYLIC_ACID, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findESTER(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::ESTER, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findAMIDE(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::AMIDE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findNITRILE(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::NITRILE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findALDEHYDE(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::ALDEHYDE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findKETONE(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::KETONE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findALCOHOL(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::ALCOHOL, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findAMINE(const std::vector<SingleAtom*>& chain)
{
    return { false, { FunctionalGroup::AMINE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findALKYNE(const std::vector<SingleAtom*>& chain)
{
    SingleAtom* tripleCarbon = nullptr;
    for (auto atom : chain)
    {
        std::vector<SingleAtom*> doubles = atom->getTripleBonds();
        if (contains(doubles, ElementType::Carbon) && contains(chain, ElementType::Carbon))
            tripleCarbon = atom;
    }
    return { bool(tripleCarbon), { FunctionalGroup::ALKYNE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findALKENE(const std::vector<SingleAtom*>& chain)
{
    SingleAtom* doubleCarbon = nullptr;
    for (auto atom : chain)
    {
        std::vector<SingleAtom*> doubles = atom->getDoubleBonds();
        if (contains(doubles, ElementType::Carbon) && contains(chain, ElementType::Carbon))
            doubleCarbon = atom;
    }
    return { bool(doubleCarbon), { FunctionalGroup::ALKENE, {} } };
}

std::pair<bool, Namer::BrokenSubstituents> Namer::findALKANE(const std::vector<SingleAtom*>& chain)
{
    BrokenSubstituents brokenSubstituents { FunctionalGroup::ALKENE, {} };
    int i = 0;
    for (auto atom : chain)
    {
        std::vector<SingleAtom*> unique = atom->getUniqueBonds();
        for (auto a : unique)
        {
            if (contains(chain, a))
                continue;
            a->remove(atom);
            brokenSubstituents.substituents.push_back({ a, i });
        }
        i++;
    }
    return { true, brokenSubstituents };
}

std::string Namer::nameOrganic(std::vector<SingleAtom*>& chain)
{
    for (SingleAtom* atom : chain)
        ((Atom*)atom->correspondent)->color = glm::vec3(1.0f);
    int size = chain.size();
    std::string prefix = namePrefix(size);
    auto [group, substituents] = findAndBreakHighestPriorityGroup(chain);

    return join(prefix, nameSuffix(group));
}

std::string Namer::nameInorganic()
{
    return "CO2";
}
