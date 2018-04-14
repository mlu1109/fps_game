#pragma once

#include <vector>
#include "../renderer/vertex.hpp"
#include "wavefront_object.hpp"

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};

Mesh getMesh(const WavefrontObject &obj);