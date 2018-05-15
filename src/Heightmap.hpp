#pragma once

#include <GL/glew.h>
#include "renderer/TGA.hpp"
#include "renderer/Vertex.hpp"

struct Heightmap
{
    int width;
    int height;
    int quadWidth;
    int quadHeight;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Heightmap(int width, int height, const std::vector<unsigned char> &data, int stride);
    Heightmap(const TGA &tga);
};
