#include "FunctionalGroup.h"

std::string namePrefix(int n)
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

std::string nameSuffix(FunctionalGroup group)
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
    bool aVowel = isVowel(a.back());
    bool bVowel = isVowel(b.front());
    if (aVowel && bVowel)
    {
        return a.substr(0, a.size() - 1) + b;
    }
    return a + b;
}
