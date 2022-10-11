#include "FunctionalGroup.h"

std::string namePrefix(int n)
{
    switch (n)
    {
    case 1:
        return "meth";
    case 2:
        return "eth";
    case 3:
        return "prop";
    case 4:
        return "but";
    case 5:
        return "pent";
    case 6:
        return "hex";
    case 7:
        return "hept";
    case 8:
        return "oct";
    case 9:
        return "non";
    case 10:
        return "dec";
    default:
        throw std::runtime_error("Don't know prefix of large chain");
    }
}

std::string nameSuffix(FunctionalGroup group, int idx)
{
    std::string strIdx = std::to_string(idx);
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
        return "-" + strIdx + "-one";
    case FunctionalGroup::ALCOHOL:
        return "-" + strIdx + "-ol";
    case FunctionalGroup::AMINE:
        return "amine";
    case FunctionalGroup::ALKYNE:
        return "-" + strIdx + "-yne";
    case FunctionalGroup::ALKENE:
        return "-" + strIdx + "-ene";
    case FunctionalGroup::ALKANE:
        return "ane";
    }
}

std::string namePrefix(FunctionalGroup group)
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

std::string join(const std::string& a, const std::string& b)
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
    if (a == "" || b == "")
        return a + b;
    bool aVowel = isVowel(a.back());
    bool bVowel = isVowel(b.front());
    if (aVowel && bVowel)
    {
        return a.substr(0, a.size() - 1) + b;
    }
    return a + b;
}
