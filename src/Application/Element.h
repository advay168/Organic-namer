#pragma once
#include "Common.h"

struct ElementType
{
    enum ElementTypeEnum : uint8_t
    {
        Hydrogen = 123,
        Carbon,
        Nitrogen,
        Oxygen
    } elementType;

    inline ElementType(ElementTypeEnum el = Hydrogen)
        : elementType(el)
    {
    }

    inline operator ElementTypeEnum()
    {
        return elementType;
    }

    // ElementType elementType;
    constexpr inline const char* name()
    {
        switch (elementType)
        {
        case Hydrogen:
            return "Hydrogen";
        case Carbon:
            return "Carbon";
        case Nitrogen:
            return "Nitrogen";
        case Oxygen:
            return "Oxygen";
        }
        throw std::runtime_error("name Unknown element");
    }
    constexpr inline const char* symbol()
    {
        switch (elementType)
        {
        case Hydrogen:
            return "H";
        case Carbon:
            return "C";
        case Nitrogen:
            return "N";
        case Oxygen:
            return "O";
        }
        throw std::runtime_error("symbol Unknown element");
    }

    constexpr inline bool hasCustomColor()
    {
        switch (elementType)
        {
        case Hydrogen:
        case Carbon:
        case Nitrogen:
            return false;
        case Oxygen:
            return true;
        }
        return false;
    }

    constexpr inline const glm::vec3 color()
    {
        switch (elementType)
        {
        case Hydrogen:
        case Carbon:
        case Nitrogen:
        case Oxygen:
            return glm::vec3(0.7f, 0.4f, 0.8f);
        }
        throw std::runtime_error("color Unknown element");
    }
};
