#include "Vertex.hpp"

bool operator<(const Vertex &lhs, const Vertex &rhs)
{
    if (lhs.position != rhs.position)
        return lhs.position < rhs.position;
    if (lhs.normal != rhs.normal)
        return lhs.normal < rhs.normal;
    if (lhs.texCoord != rhs.texCoord)
        return lhs.texCoord < rhs.texCoord;
    if (lhs.color != rhs.color)
        return lhs.color < rhs.color;

    return false; // equal
}
