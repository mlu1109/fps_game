#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position; // x,y,z
    glm::vec3 normal;   // x,y,z
    glm::vec2 texCoord; // u,v
};

bool operator<(const Vertex &lhs, const Vertex &rhs);