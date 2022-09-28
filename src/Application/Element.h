#pragma once
#include "Common.h"

struct Element {
  enum ElementType : uint8_t { Hydrogen = 123, Carbon, Nitrogen, Oxygen };
  ElementType elementType;
  constexpr inline const char *name() {
    switch (elementType) {
    case Element::Hydrogen:
      return "Hydrogen";
    case Element::Carbon:
      return "Carbon";
    case Element::Nitrogen:
      return "Nitrogen";
    case Element::Oxygen:
      return "Oxygen";
    }
    throw std::runtime_error("name Unknown element");
  }
  constexpr inline const char *symbol() {
    switch (elementType) {
    case Element::Hydrogen:
      return "H";
    case Element::Carbon:
      return "C";
    case Element::Nitrogen:
      return "N";
    case Element::Oxygen:
      return "O";
    }
    throw std::runtime_error("symbol Unknown element");
  }

  constexpr inline bool hasCustomColor() {
    switch (elementType) {
      // case Element::Hydrogen:
      // case Element::Carbon:
      // case Element::Nitrogen:
    case Element::Oxygen:
      return true;
    }
    return false;
  }

  constexpr inline const glm::vec3 color() {
    switch (elementType) {
    case Element::Hydrogen:
    case Element::Carbon:
    case Element::Nitrogen:
    case Element::Oxygen:
      return glm::vec3(0.7f, 0.4f, 0.8f);
    }
    throw std::runtime_error("color Unknown element");
  }
};
