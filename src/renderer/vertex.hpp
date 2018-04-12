#pragma once

#include <GL/glew.h>

struct Vertex
{
    GLfloat position[3]; // x,y,z
    GLfloat normal[3];   // x,y,z
    GLbyte color[4];     // r,g,b,a [0,255]
    GLfloat texCoord[2]; // u,v
};