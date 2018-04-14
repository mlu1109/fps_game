#pragma once

#include <array>
#include <GL/glew.h>

struct Vertex
{
    std::array<GLfloat, 3> position; // x,y,z
    std::array<GLfloat, 3> normal;   // x,y,z
    std::array<GLbyte, 4> color;     // r,g,b,a [0,255]
    std::array<GLfloat, 2> texCoord; // u,v
};