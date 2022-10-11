#include "SingleAtom.h"

std::vector<SingleAtom*> SingleAtom::getSingleBonds()
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

std::vector<SingleAtom*> SingleAtom::getDoubleBonds()
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

std::vector<SingleAtom*> SingleAtom::getTripleBonds()
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
std::vector<SingleAtom*> SingleAtom::findAtoms(ElementType::ElementTypeEnum el)
{
    std::vector<SingleAtom*> ret;
    for (auto& ba : bondedAtoms)
    {
        if (ba.bondedAtom->element == el)
            ret.push_back(ba.bondedAtom);
    }
    return ret;
}

std::vector<SingleAtom*> SingleAtom::getUniqueBonds()
{
    std::vector<SingleAtom*> result;
    for (auto& ba : bondedAtoms)
    {
        if (!ba.isSameBond)
            result.push_back(ba.bondedAtom);
    }
    return result;
}

int SingleAtom::remove(SingleAtom* toRemove)
{
    int count = 0;
    for (auto [a, x] : bondedAtoms)
    {
        count += a == toRemove;
    }
    bondedAtoms.erase(std::remove_if(bondedAtoms.begin(), bondedAtoms.end(), [&](BondedAtom x)
                                     { return x.bondedAtom == toRemove; }),
                      bondedAtoms.end());
    return count;
}
