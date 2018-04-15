#pragma once

#include <vector>
#include "loaders/obj.hpp"
#include "vertex.hpp"

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};

Mesh makeMesh(const OBJ &obj);