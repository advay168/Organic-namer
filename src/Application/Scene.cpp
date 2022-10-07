#include "Scene.h"

void Scene::deserialize(std::istream& stream)
{
    std::vector<Atom*> indexedAtoms;
    while (stream.peek() != '$')
    {
        ElementType el;
        uint16_t temp;
        stream >> temp;
        el.elementType = (ElementType::ElementTypeEnum) temp;

        float px, py;
        stream >> px >> py;

        if (stream.get() != '\n')
        {
            throw std::runtime_error("Malformed input to scene deserializer. Expected \\n at end of atom data.");
        }

        Atom a(el);
        a.pos = { px, py };
        atoms.push_back(std::move(a));
        indexedAtoms.push_back(&atoms.back());
    }
    if (stream.get() != '$' || stream.get() != '\n')
    {
        throw std::runtime_error("Malformed input to scene deserializer");
    }

    while (stream.peek() != std::char_traits<char>::eof())
    {
        uint64_t aIdx, bIdx;
        uint16_t count;

        stream >> aIdx >> bIdx >> count;

        Bond b(indexedAtoms[aIdx], indexedAtoms[bIdx]);
        b.count = count;
        bonds.push_back(b);

        if (stream.get() != '\n')
        {
            throw std::runtime_error("Malformed input to scene deserializer");
        }
    }
}

void Scene::serialize(std::ostream& stream)
{
    uint64_t idx = 0;
    std::map<Atom*, uint64_t> mapping;
    for (Atom& atom : atoms)
    {
        stream << (uint16_t)atom.element.elementType << " ";
        glm::vec2& p = atom.pos;
        stream << p.x << " " << p.y;
        stream << "\n";

        mapping[&atom] = idx;
        idx++;
    }

    stream << "$\n";

    for (Bond& bond : bonds)
    {
        uint64_t aIdx = mapping[bond.a];
        uint64_t bIdx = mapping[bond.b];

        stream << aIdx << " ";
        stream << bIdx << " ";
        stream << (uint16_t)bond.count << "\n";
    }
}
