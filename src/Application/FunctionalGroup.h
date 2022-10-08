#pragma once
#include "Common.h"

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

std::string namePrefix(int n);
std::string namePrefix(FunctionalGroup group);
std::string nameSuffix(FunctionalGroup group);
std::string join(const std::string& a, const std::string& b);
