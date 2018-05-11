#include "Vertex.hpp"

bool operator<(const glm::vec3 &lhs, const glm::vec3 &rhs)
{
    if (lhs.x < rhs.x)
        return true;
    else if (lhs.x > rhs.x)
        return false;

    if (lhs.y < rhs.y)
        return true;
    else if (lhs.y > rhs.y)
        return false;

    if (lhs.z < rhs.z)
        return true;
    else if (lhs.z > rhs.z)
        return false;

    return false;
}

bool operator<(const glm::vec2 &lhs, const glm::vec2 &rhs)
{
    if (lhs.x < rhs.x)
        return true;
    else if (lhs.x > rhs.x)
        return false;

    if (lhs.y < rhs.y)
        return true;
    else if (lhs.x > rhs.y)
        return false;

    return false;
}

bool operator<(const Vertex &lhs, const Vertex &rhs)
{
    if (lhs.position != rhs.position)
        return lhs.position < rhs.position;
    if (lhs.normal != rhs.normal)
        return lhs.normal < rhs.normal;
    if (lhs.texCoord != rhs.texCoord)
        return lhs.texCoord < rhs.texCoord;

    return false; // equal
}
