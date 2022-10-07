#include "Namer.h"
#include <array>
#include <functional>
#include <glm/gtc/constants.hpp>

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
    std::vector<Namer::SingleAtom*> maxPath;
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
        for (SingleAtom::BondedAtom ba : copy)
        {
            if (ba.bondedAtom->element == ElementType::Carbon)
            {
                atom->bondedAtoms.push_back(ba);
            }
        }
        for (SingleAtom::BondedAtom ba : copy)
        {
            if (ba.bondedAtom->element == ElementType::Nitrogen)
            {
                atom->bondedAtoms.push_back(ba);
            }
        }
        for (SingleAtom::BondedAtom ba : copy)
        {
            if (ba.bondedAtom->element == ElementType::Oxygen)
            {
                atom->bondedAtoms.push_back(ba);
            }
        }
        for (SingleAtom::BondedAtom ba : copy)
        {
            if (ba.bondedAtom->element == ElementType::Hydrogen)
            {
                atom->bondedAtoms.push_back(ba);
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

std::vector<Namer::SingleAtom*> Namer::SingleAtom::getSingleBonds()
{
    auto a = bondedAtoms[0];
    auto b = bondedAtoms[1];
    auto c = bondedAtoms[2];
    auto d = bondedAtoms[3];
    std::vector<SingleAtom*> result;
    if (!b.isSameBond) // X Y _ _
        result.push_back(a.bondedAtom);
    if (!b.isSameBond && !c.isSameBond) // X Y Z _
        result.push_back(b.bondedAtom);
    if (!c.isSameBond && !d.isSameBond) // _ X Y Z
        result.push_back(c.bondedAtom);
    if (!d.isSameBond) // _ _ X Y
        result.push_back(d.bondedAtom);
    return result;
}

std::vector<Namer::SingleAtom*> Namer::SingleAtom::getDoubleBonds()
{
    auto a = bondedAtoms[0];
    auto b = bondedAtoms[1];
    auto c = bondedAtoms[2];
    auto d = bondedAtoms[3];
    std::vector<SingleAtom*> result;
    if (b.isSameBond && !c.isSameBond) // X X Y _
        result.push_back(a.bondedAtom);
    if (!b.isSameBond && c.isSameBond && !d.isSameBond) // X Y Y Z
        result.push_back(b.bondedAtom);
    if (!c.isSameBond && d.isSameBond) // _ X Y Y
        result.push_back(c.bondedAtom);
    return result;
}

std::vector<Namer::SingleAtom*> Namer::SingleAtom::getTripleBonds()
{
    auto a = bondedAtoms[0];
    auto b = bondedAtoms[1];
    auto c = bondedAtoms[2];
    auto d = bondedAtoms[3];
    std::vector<SingleAtom*> result;
    if (b.isSameBond && c.isSameBond && !d.isSameBond)
        result.push_back(a.bondedAtom);
    if (!b.isSameBond && c.isSameBond && d.isSameBond)
        result.push_back(b.bondedAtom);
    return result;
}
std::vector<Namer::SingleAtom*> Namer::SingleAtom::findAtoms(ElementType::ElementTypeEnum el)
{
    std::vector<SingleAtom*> ret;
    for (auto& ba : bondedAtoms)
    {
        if (ba.bondedAtom->element == el)
            ret.push_back(ba.bondedAtom);
    }
    return ret;
}

std::vector<Namer::SingleAtom::BondedAtom> Namer::SingleAtom::getUniqueBonds()
{
    std::vector<BondedAtom> result;
    for (auto& ba : bondedAtoms)
    {
        if (!ba.isSameBond)
            result.push_back(ba);
    }
    return result;
}
std::vector<Namer::SingleAtom*> Namer::findCarbonAtoms()
{
    std::vector<Namer::SingleAtom*> atoms;
    for (auto& atom : atomsList)
    {
        if (atom->isCarbon())
            atoms.push_back(atom.get());
    }
    return atoms;
}

Namer::SingleAtom* Namer::findTerminalCarbonAtom(SingleAtom* carbonAtom)
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

std::vector<Namer::SingleAtom*> Namer::findMaxCarbonChain(SingleAtom* carbonAtom)
{
    for (auto& atom : atomsList)
    {
        atom->visited = false;
    }
    using ret_t = std::vector<Namer::SingleAtom*>;
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
    return { false, { FunctionalGroup::ALKYNE, {} } };
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
    return { true, { FunctionalGroup::ALKANE, {} } };
}

std::string Namer::nameOrganic(std::vector<Namer::SingleAtom*>& chain)
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

std::string Namer::namePrefix(int n)
{
    switch (n)
    {
    case 1:
        return "metha";
    case 2:
        return "etha";
    case 3:
        return "propa";
    case 4:
        return "buta";
    case 5:
        return "penta";
    case 6:
        return "hexa";
    case 7:
        return "hepta";
    case 8:
        return "octa";
    case 9:
        return "nona";
    case 10:
        return "deca";
    default:
        throw std::runtime_error("Don't know prefix of large chain");
    }
}

std::string Namer::nameSuffix(FunctionalGroup group)
{
    switch (group)
    {
    case FunctionalGroup::CARBOXYLIC_ACID:
        return "noic acid";
    case FunctionalGroup::ESTER:
        return "oate";
    case FunctionalGroup::AMIDE:
        return "amide";
    case FunctionalGroup::NITRILE:
        return "nitrile";
    case FunctionalGroup::ALDEHYDE:
        return "al";
    case FunctionalGroup::KETONE:
        return "one";
    case FunctionalGroup::ALCOHOL:
        return "ol";
    case FunctionalGroup::AMINE:
        return "amine";
    case FunctionalGroup::ALKYNE:
        return "yne";
    case FunctionalGroup::ALKENE:
        return "ene";
    case FunctionalGroup::ALKANE:
        return "ane";
    }
}

std::string Namer::namePrefix(FunctionalGroup group)
{
    switch (group)
    {
    case FunctionalGroup::CARBOXYLIC_ACID:
        return "";
    case FunctionalGroup::ESTER:
        return "";
    case FunctionalGroup::AMIDE:
        return "amido";
    case FunctionalGroup::NITRILE:
        return "";
    case FunctionalGroup::ALDEHYDE:
        return "";
    case FunctionalGroup::KETONE:
        return "";
    case FunctionalGroup::ALCOHOL:
        return "hydroxy";
    case FunctionalGroup::AMINE:
        return "amino";
    case FunctionalGroup::ALKYNE:
        return "";
    case FunctionalGroup::ALKENE:
        return "";
    case FunctionalGroup::ALKANE:
        return "yl";
    }
}

std::string Namer::join(const std::string& a, const std::string& b)
{
    auto isVowel = [](char c)
    {
        switch (c)
        {
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'i':
        case 'I':
        case 'o':
        case 'O':
        case 'u':
        case 'U':
        case 'y':
        case 'Y':
            return true;
        default:
            return false;
        }
    };
    bool aVowel = isVowel(a.back());
    bool bVowel = isVowel(b.front());
    if (aVowel && bVowel)
    {
        return a.substr(0, a.size() - 1) + b;
    }
    return a + b;
}
