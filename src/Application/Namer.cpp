#include "Namer.h"
#include <functional>

Namer::Namer(const Scene& scene)
{
    for (const Atom& atom : scene.atoms)
    {
        SingleAtom newAtom { atom.element, &atom };
        atomsList.push_back(std::make_unique<SingleAtom>(newAtom));
        atomMapping[&atom] = atomsList.back().get();
    }

    for (auto [a, b, count] : scene.bonds)
    {
        auto atomA = atomMapping[a];
        auto atomB = atomMapping[b];
        atomA->bondedAtoms.push_back({ atomB, false });
        atomB->bondedAtoms.push_back({ atomA, false });
        for (count--; count > 0; count--)
        {
            atomA->bondedAtoms.push_back({ atomB, true });
            atomB->bondedAtoms.push_back({ atomA, true });
        }
    }
    for (auto& atom : atomsList)
    {
        if (atom->isHydrogen() && atom->bondedAtoms.size() != 1)
            throw std::runtime_error("Hydrogen atom does not have 1 bond.");
        if (!atom->isHydrogen() && atom->bondedAtoms.size() != 4)
            throw std::runtime_error("Atom does not have 4 bonds.");
    }
    sortBonds();
}

std::string Namer::getName()
{
    SingleAtom* carbonAtom = findCarbonAtom();
    carbonAtom = findTerminalCarbonAtom(carbonAtom);
    if (carbonAtom)
        return nameOrganic(carbonAtom);
    return nameInorganic();
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

std::vector<std::tuple<Namer::SingleAtom::BondedAtom>> Namer::SingleAtom::getSingleBonds()
{
    auto a = bondedAtoms[0];
    auto b = bondedAtoms[1];
    auto c = bondedAtoms[2];
    auto d = bondedAtoms[3];
    std::vector<std::tuple<BondedAtom>> result;
    if (!b.isSameBond) // X Y _ _
        result.push_back({ a });
    if (!b.isSameBond && !c.isSameBond) // X Y Z _
        result.push_back({ b });
    if (!c.isSameBond && !d.isSameBond) // _ X Y Z
        result.push_back({ c });
    if (!d.isSameBond) // _ _ X Y
        result.push_back({ d });
    return result;
}

std::vector<std::tuple<Namer::SingleAtom::BondedAtom, Namer::SingleAtom::BondedAtom>> Namer::SingleAtom::getDoubleBonds()
{
    auto a = bondedAtoms[0];
    auto b = bondedAtoms[1];
    auto c = bondedAtoms[2];
    auto d = bondedAtoms[3];
    std::vector<std::tuple<BondedAtom, BondedAtom>> result;
    if (b.isSameBond && !c.isSameBond) // X X Y _
        result.push_back({ a, b });
    if (!b.isSameBond && c.isSameBond && !d.isSameBond) // X Y Y Z
        result.push_back({ b, c });
    if (!c.isSameBond && d.isSameBond) // _ X Y Y
        result.push_back({ c, d });
    return result;
}

std::vector<std::tuple<Namer::SingleAtom::BondedAtom, Namer::SingleAtom::BondedAtom, Namer::SingleAtom::BondedAtom>> Namer::SingleAtom::getTripleBonds()
{
    auto a = bondedAtoms[0];
    auto b = bondedAtoms[1];
    auto c = bondedAtoms[2];
    auto d = bondedAtoms[3];
    std::vector<std::tuple<BondedAtom, BondedAtom, BondedAtom>> result;
    if (b.isSameBond && c.isSameBond && !d.isSameBond)
        result.push_back({ a, b, c });
    if (!b.isSameBond && c.isSameBond && d.isSameBond)
        result.push_back({ b, c, d });
    return result;
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
Namer::SingleAtom* Namer::findCarbonAtom()
{
    for (auto& atom : atomsList)
    {
        if (atom->isCarbon())
            return atom.get();
    }
    return nullptr;
}

Namer::SingleAtom* Namer::findTerminalCarbonAtom(SingleAtom* carbonAtom)
{
    for (auto& atom : atomsList)
    {
        atom->visited = false;
    }
    std::function<SingleAtom*(SingleAtom*)> helper = [&helper](auto carbonAtom)
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

std::string Namer::nameOrganic(SingleAtom* carbonAtom)
{
    ((Atom*)carbonAtom->correspondent)->color = glm::vec3(1.0f);
    return "CH4";
}

std::string Namer::nameInorganic()
{
    return "CO2";
}
